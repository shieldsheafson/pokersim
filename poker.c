# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# define SumConsecInts(lowestInt, highestInt) (((highestInt - lowestInt + 1) * (highestInt + lowestInt))/2)

static int sortedListHasDuplicates(int list[], int listLength) {
    for (int i = 0; i < listLength-1; i++) {
        if (list[i] == list[i+1]) {
            return 1;
        }
    }

    return 0;
}

static int inList(int item, int list[], int listLength) {
    for (int i = 0; i < listLength; i++) {
        if (item == list[i]) {
            return 1;
        }
    }

    return 0;
}

static void insert(int list[], int listLength, int index, int item) {
    for (int i = listLength - 2; i >= index; i--) {
        list[i + 1] = list[i];
    }
    list[index] = item;
}

static void insertionSort(int list[], int listLength, int sortedList[]) {
    sortedList[0] = list[0];
    for (int i = 1; i < listLength; i++) {
        for (int j = 0; j <= i; j++) {
            if (list[i] <= sortedList[j] || j == i)  {
                insert(sortedList, listLength, j, list[i]);
                break;
            }
        }
    }
}

static void listOfRandomIndices(int listOfIndices[], int numIndices) {
    for (int i = 0; i < numIndices; i++) {
        listOfIndices[i] = -1;
    }

    int k = 1;
    int num = (int)arc4random_uniform(50);
    listOfIndices[0] = num;
    while (k < numIndices) {
        num = (int)arc4random_uniform(50);
        if (!inList(num, listOfIndices, numIndices)) {
            listOfIndices[k] = num;
            k++;
        }
    }
}

static void findOtherValues(int list[], int listLength, int value, int otherValues[]) {
    int n = 0;
    for (int i = 0; i < listLength; i ++) {
        if (list[i] != value) {
            otherValues[n] = list[i];
            n += 1;
        }
    }
} 

static void findDuplicates(int pool[], int poolSize, int duplicateCards[]) {

    int duplicates[13] = {0};
    for (int i = 0; i < poolSize; i++) {
        duplicates[pool[i]-2] += 1;
        switch (duplicates[pool[i]-2]) {
            case 4:
                duplicateCards[3] = pool[i];
                break;
            case 3:
                duplicateCards[2] = pool[i]; 
                break;
            case 2: 
                if (pool[i] > duplicateCards[1]) {
                    duplicateCards[0] = duplicateCards[1];
                    duplicateCards[1] = pool[i];
                } else if (pool[i] > duplicateCards[0]) {
                    duplicateCards[0] = pool[i];
                }
                break;
        }
    }
}

static int encodeCardValues(int list[], int listSize) {
    int encodedValue = 0;
    for (int i = 0; i < listSize; i++) {
        double power = pow(10, 10+(i-listSize)*2);
        encodedValue += list[i] * (int)power;
    }

    return encodedValue;
}

static long highCard(int pool[], int poolSize) {
    return encodeCardValues(pool, poolSize);
}

static long pair(int pool[], int poolSize, int duplicateCards[]) {
    int score[4];
    if (duplicateCards[1] > 0) {
        score[3] = duplicateCards[1];
        findOtherValues(pool, poolSize, duplicateCards[1], score);
        return encodeCardValues(score, 4) + 10000000000;
    }
    
    return 0;
}

static long twoPair(int pool[], int poolSize, int duplicateCards[]) {
    int score[3];
    if (duplicateCards[1] > 0 && duplicateCards[0] > 0) {
        findOtherValues(pool, poolSize, duplicateCards[1], score);
        findOtherValues(score, 3, duplicateCards[0], score);
        score[2] = duplicateCards[1];
        score[1] = duplicateCards[0];
        return encodeCardValues(score, 3) + 20000000000;
    }
    
    return 0;
}

static long threeOfAKind(int pool[], int poolSize, int duplicateCards[]) {
    int score[3];
    if (duplicateCards[2] > 0) {
        score[2] = duplicateCards[2];
        findOtherValues(pool, poolSize, duplicateCards[2], score);
        return encodeCardValues(score, 3) + 30000000000;
    }
    
    return 0;
}

static long straight(int pool[], int poolSize, int hasDuplicates) {

    // check for low ace straight
    if (pool[4] == 14 && pool[0] == 2 && pool[1] == 3 && pool[2] == 4 && pool[3] == 5) {
        return 40500000000;
    }

    // find sum of pool
    int sumOfPool = 0;
    for (int i = 0; i < poolSize; i++) {
        sumOfPool += pool[i];
    }

    // check if the sum of the pool is equal to 
    // the sum of consecutive intergers from the 
    // lowest number in the pool to the highest
    // if so there must be a straight
    if (sumOfPool == SumConsecInts(pool[0], pool[poolSize - 1]) && !hasDuplicates) {
        return pool[poolSize - 1]*100000000 + 40000000000;
    }

    return 0;
}

static long flush(int pool[], int suits[], int poolSize) {

    for (int i = 1; i < poolSize; i++) {
        if (suits[i] != suits[0]) {
            return 0;
        }
    }

    return encodeCardValues(pool, poolSize) + 50000000000;
}

static long fullHouse(int duplicateCards[]) {
    if (duplicateCards[2] > 0 && (duplicateCards[1] != duplicateCards[2] && duplicateCards[1] > 0)) {
        return duplicateCards[2] * 100 + duplicateCards[1] + 60000000000;
    } else if (duplicateCards[2] > 0 && (duplicateCards[0] != duplicateCards[2] && duplicateCards[0] > 0)) {
        return duplicateCards[2] * 100 + duplicateCards[0] + 60000000000;
    }
    
    return 0;
}

static long fourOfAKind(int pool[], int poolSize, int duplicateCards[]) {
    int score[1];
    if (duplicateCards[3] > 0) {
        findOtherValues(pool, poolSize, duplicateCards[1], score);
        return duplicateCards[3] * 100 + score[0] + 70000000000;
    }
    
    return 0;
}

static long straightFlush(long straightReturnValue, long flushReturnValue) {
    if (straightReturnValue > 0 && flushReturnValue > 0) {
        return (straightReturnValue - 40000000000) + 80000000000;
    }
    return 0;
}

static long returnScore(int hand[], int handLength, int suits[]) {
    int duplicateCards[4] = {0};
    findDuplicates(hand, handLength, duplicateCards);
    long straightReturn = straight(hand, handLength, sortedListHasDuplicates(hand, handLength));
    long flushReturn = flush(hand, suits, handLength);

    long scores[9] = {highCard(hand, handLength), 
                      pair(hand, handLength, duplicateCards), 
                      twoPair(hand, handLength, duplicateCards), 
                      threeOfAKind(hand, handLength, duplicateCards), 
                      straightReturn, 
                      flushReturn,
                      fullHouse(duplicateCards),
                      fourOfAKind(hand, handLength, duplicateCards),
                      straightFlush(straightReturn, flushReturn)};

    for (int i = 8; i > -1; i--) {
        if (scores[i] != 0) {
            return scores[i];
        }
    }

    return 0;
}

static long findBestScore(int cardsInPlay[]) {
    long score = 0;
    long tempScore;
    int suits[7];
    int hand[5];
    int suitsInHand[5];

    for (int i = 0; i < 7; i++) {
        suits[i] = (cardsInPlay[i] % 10);
        cardsInPlay[i] = (cardsInPlay[i] / 10);
    }

    // some real ugly code, need some more math to fix it
    // its all 21 combinations of 7 choose 5

    hand[0] = cardsInPlay[0]; 
    hand[1] = cardsInPlay[1]; 
    hand[2] = cardsInPlay[2];
    hand[3] = cardsInPlay[3];
    hand[4] = cardsInPlay[4];

    suitsInHand[0] = suits[0]; 
    suitsInHand[1] = suits[1]; 
    suitsInHand[2] = suits[2];
    suitsInHand[3] = suits[3];
    suitsInHand[4] = suits[4];

    tempScore = returnScore(hand, 5, suitsInHand);
    if (tempScore > score) {
        score = tempScore;
    }

    hand[0] = cardsInPlay[0]; 
    hand[1] = cardsInPlay[1]; 
    hand[2] = cardsInPlay[2];
    hand[3] = cardsInPlay[3];
    hand[4] = cardsInPlay[5];

    suitsInHand[0] = suits[0]; 
    suitsInHand[1] = suits[1]; 
    suitsInHand[2] = suits[2];
    suitsInHand[3] = suits[3];
    suitsInHand[4] = suits[5];

    tempScore = returnScore(hand, 5, suitsInHand);
    if (tempScore > score) {
        score = tempScore;
    }

    hand[0] = cardsInPlay[0]; 
    hand[1] = cardsInPlay[1]; 
    hand[2] = cardsInPlay[2];
    hand[3] = cardsInPlay[3];
    hand[4] = cardsInPlay[6];

    suitsInHand[0] = suits[0]; 
    suitsInHand[1] = suits[1]; 
    suitsInHand[2] = suits[2];
    suitsInHand[3] = suits[3];
    suitsInHand[4] = suits[6];

    tempScore = returnScore(hand, 5, suitsInHand);
    if (tempScore > score) {
        score = tempScore;
    }

    hand[0] = cardsInPlay[0]; 
    hand[1] = cardsInPlay[1]; 
    hand[2] = cardsInPlay[2];
    hand[3] = cardsInPlay[4];
    hand[4] = cardsInPlay[5];

    suitsInHand[0] = suits[0]; 
    suitsInHand[1] = suits[1]; 
    suitsInHand[2] = suits[2];
    suitsInHand[3] = suits[4];
    suitsInHand[4] = suits[5];

    tempScore = returnScore(hand, 5, suitsInHand);
    if (tempScore > score) {
        score = tempScore;
    }

    hand[0] = cardsInPlay[0]; 
    hand[1] = cardsInPlay[1]; 
    hand[2] = cardsInPlay[2];
    hand[3] = cardsInPlay[4];
    hand[4] = cardsInPlay[6];

    suitsInHand[0] = suits[0]; 
    suitsInHand[1] = suits[1]; 
    suitsInHand[2] = suits[2];
    suitsInHand[3] = suits[4];
    suitsInHand[4] = suits[6];

    tempScore = returnScore(hand, 5, suitsInHand);
    if (tempScore > score) {
        score = tempScore;
    }

    hand[0] = cardsInPlay[0]; 
    hand[1] = cardsInPlay[1]; 
    hand[2] = cardsInPlay[2];
    hand[3] = cardsInPlay[5];
    hand[4] = cardsInPlay[6];

    suitsInHand[0] = suits[0]; 
    suitsInHand[1] = suits[1]; 
    suitsInHand[2] = suits[2];
    suitsInHand[3] = suits[5];
    suitsInHand[4] = suits[6];

    tempScore = returnScore(hand, 5, suitsInHand);
    if (tempScore > score) {
        score = tempScore;
    }

    hand[0] = cardsInPlay[0]; 
    hand[1] = cardsInPlay[1]; 
    hand[2] = cardsInPlay[3];
    hand[3] = cardsInPlay[4];
    hand[4] = cardsInPlay[5];
    
    suitsInHand[0] = suits[0]; 
    suitsInHand[1] = suits[1]; 
    suitsInHand[2] = suits[3];
    suitsInHand[3] = suits[4];
    suitsInHand[4] = suits[5];

    tempScore = returnScore(hand, 5, suitsInHand);
    if (tempScore > score) {
        score = tempScore;
    }

    hand[0] = cardsInPlay[0]; 
    hand[1] = cardsInPlay[1]; 
    hand[2] = cardsInPlay[3];
    hand[3] = cardsInPlay[4];
    hand[4] = cardsInPlay[6];
    
    suitsInHand[0] = suits[0]; 
    suitsInHand[1] = suits[1]; 
    suitsInHand[2] = suits[3];
    suitsInHand[3] = suits[4];
    suitsInHand[4] = suits[6];

    tempScore = returnScore(hand, 5, suitsInHand);
    if (tempScore > score) {
        score = tempScore;
    }

    hand[0] = cardsInPlay[0]; 
    hand[1] = cardsInPlay[1]; 
    hand[2] = cardsInPlay[3];
    hand[3] = cardsInPlay[5];
    hand[4] = cardsInPlay[6];
    
    suitsInHand[0] = suits[0]; 
    suitsInHand[1] = suits[1]; 
    suitsInHand[2] = suits[3];
    suitsInHand[3] = suits[5];
    suitsInHand[4] = suits[6];

    tempScore = returnScore(hand, 5, suitsInHand);
    if (tempScore > score) {
        score = tempScore;
    }

    hand[0] = cardsInPlay[0]; 
    hand[1] = cardsInPlay[1]; 
    hand[2] = cardsInPlay[4];
    hand[3] = cardsInPlay[5];
    hand[4] = cardsInPlay[6];
    
    suitsInHand[0] = suits[0]; 
    suitsInHand[1] = suits[1]; 
    suitsInHand[2] = suits[4];
    suitsInHand[3] = suits[5];
    suitsInHand[4] = suits[6];

    tempScore = returnScore(hand, 5, suitsInHand);
    if (tempScore > score) {
        score = tempScore;
    }

    hand[0] = cardsInPlay[0]; 
    hand[1] = cardsInPlay[2]; 
    hand[2] = cardsInPlay[3];
    hand[3] = cardsInPlay[4];
    hand[4] = cardsInPlay[5];
    
    suitsInHand[0] = suits[0]; 
    suitsInHand[1] = suits[2]; 
    suitsInHand[2] = suits[3];
    suitsInHand[3] = suits[4];
    suitsInHand[4] = suits[5];

    tempScore = returnScore(hand, 5, suitsInHand);
    if (tempScore > score) {
        score = tempScore;
    }

    hand[0] = cardsInPlay[0]; 
    hand[1] = cardsInPlay[2]; 
    hand[2] = cardsInPlay[3];
    hand[3] = cardsInPlay[5];
    hand[4] = cardsInPlay[6];
    
    suitsInHand[0] = suits[0]; 
    suitsInHand[1] = suits[2]; 
    suitsInHand[2] = suits[3];
    suitsInHand[3] = suits[5];
    suitsInHand[4] = suits[6];

    tempScore = returnScore(hand, 5, suitsInHand);
    if (tempScore > score) {
        score = tempScore;
    }

    hand[0] = cardsInPlay[0]; 
    hand[1] = cardsInPlay[2]; 
    hand[2] = cardsInPlay[3];
    hand[3] = cardsInPlay[4];
    hand[4] = cardsInPlay[6];
    
    suitsInHand[0] = suits[0]; 
    suitsInHand[1] = suits[2]; 
    suitsInHand[2] = suits[3];
    suitsInHand[3] = suits[4];
    suitsInHand[4] = suits[6];

    tempScore = returnScore(hand, 5, suitsInHand);
    if (tempScore > score) {
        score = tempScore;
    }

    hand[0] = cardsInPlay[0]; 
    hand[1] = cardsInPlay[2]; 
    hand[2] = cardsInPlay[4];
    hand[3] = cardsInPlay[5];
    hand[4] = cardsInPlay[6];
    
    suitsInHand[0] = suits[0]; 
    suitsInHand[1] = suits[2]; 
    suitsInHand[2] = suits[4];
    suitsInHand[3] = suits[5];
    suitsInHand[4] = suits[6];

    tempScore = returnScore(hand, 5, suitsInHand);
    if (tempScore > score) {
        score = tempScore;
    }

    hand[0] = cardsInPlay[0]; 
    hand[1] = cardsInPlay[3]; 
    hand[2] = cardsInPlay[4];
    hand[3] = cardsInPlay[5];
    hand[4] = cardsInPlay[6];
    
    suitsInHand[0] = suits[0]; 
    suitsInHand[1] = suits[3]; 
    suitsInHand[2] = suits[4];
    suitsInHand[3] = suits[5];
    suitsInHand[4] = suits[6];

    tempScore = returnScore(hand, 5, suitsInHand);
    if (tempScore > score) {
        score = tempScore;
    }

    hand[0] = cardsInPlay[1]; 
    hand[1] = cardsInPlay[2]; 
    hand[2] = cardsInPlay[3];
    hand[3] = cardsInPlay[4];
    hand[4] = cardsInPlay[5];
    
    suitsInHand[0] = suits[1]; 
    suitsInHand[1] = suits[2]; 
    suitsInHand[2] = suits[3];
    suitsInHand[3] = suits[4];
    suitsInHand[4] = suits[5];

    tempScore = returnScore(hand, 5, suitsInHand);
    if (tempScore > score) {
        score = tempScore;
    }

    hand[0] = cardsInPlay[1]; 
    hand[1] = cardsInPlay[2]; 
    hand[2] = cardsInPlay[3];
    hand[3] = cardsInPlay[5];
    hand[4] = cardsInPlay[6];
    
    suitsInHand[0] = suits[1]; 
    suitsInHand[1] = suits[2]; 
    suitsInHand[2] = suits[3];
    suitsInHand[3] = suits[5];
    suitsInHand[4] = suits[6];

    tempScore = returnScore(hand, 5, suitsInHand);
    if (tempScore > score) {
        score = tempScore;
    }

    hand[0] = cardsInPlay[1]; 
    hand[1] = cardsInPlay[2]; 
    hand[2] = cardsInPlay[3];
    hand[3] = cardsInPlay[4];
    hand[4] = cardsInPlay[6];
    
    suitsInHand[0] = suits[1]; 
    suitsInHand[1] = suits[2]; 
    suitsInHand[2] = suits[3];
    suitsInHand[3] = suits[4];
    suitsInHand[4] = suits[6];

    tempScore = returnScore(hand, 5, suitsInHand);
    if (tempScore > score) {
        score = tempScore;
    }

    hand[0] = cardsInPlay[1]; 
    hand[1] = cardsInPlay[2]; 
    hand[2] = cardsInPlay[4];
    hand[3] = cardsInPlay[5];
    hand[4] = cardsInPlay[6];
    
    suitsInHand[0] = suits[1]; 
    suitsInHand[1] = suits[2]; 
    suitsInHand[2] = suits[4];
    suitsInHand[3] = suits[5];
    suitsInHand[4] = suits[6];

    tempScore = returnScore(hand, 5, suitsInHand);
    if (tempScore > score) {
        score = tempScore;
    }

    hand[0] = cardsInPlay[1]; 
    hand[1] = cardsInPlay[3]; 
    hand[2] = cardsInPlay[4];
    hand[3] = cardsInPlay[5];
    hand[4] = cardsInPlay[6];
    
    suitsInHand[0] = suits[1]; 
    suitsInHand[1] = suits[3]; 
    suitsInHand[2] = suits[4];
    suitsInHand[3] = suits[5];
    suitsInHand[4] = suits[6];

    tempScore = returnScore(hand, 5, suitsInHand);
    if (tempScore > score) {
        score = tempScore;
    }

    hand[0] = cardsInPlay[2]; 
    hand[1] = cardsInPlay[3]; 
    hand[2] = cardsInPlay[4];
    hand[3] = cardsInPlay[5];
    hand[4] = cardsInPlay[6];
    
    suitsInHand[0] = suits[2]; 
    suitsInHand[1] = suits[3]; 
    suitsInHand[2] = suits[4];
    suitsInHand[3] = suits[5];
    suitsInHand[4] = suits[6];

    tempScore = returnScore(hand, 5, suitsInHand);
    if (tempScore > score) {
        score = tempScore;
    }

    return score;
}

static void sim(int numOtherPlayers, int hero[], int numRep, double winsLosesTies[]) {

    // create list of cards
    int cards[50];
    int n = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 2; j < 15; j++) {
            if (inList(j * 10 + i, hero, 2) == 0) {
                cards[n] = j * 10 + i;
                n += 1;
            }
        }
    }

    int numRandomIndices = 5 + numOtherPlayers * 2;
    int otherPlayerHands[(numOtherPlayers)*2];
    int listOfRandomNumbers[numRandomIndices];
    int cardsInPlay[7];
    int cardsInPlayToScore[7];

    long heroScore;
    long otherPlayerScore;

    double wins = 0;
    double loses = 0;
    
    for (int i = 0; i < numRep; i++) {
        listOfRandomIndices(listOfRandomNumbers, numRandomIndices);

        for (int j = 0; j < 5; j++) {
            cardsInPlay[j] = cards[listOfRandomNumbers[j]];
        }
        for (int j = 0; j < numOtherPlayers*2; j++) {
            otherPlayerHands[j] = cards[listOfRandomNumbers[j + 5]];
        }

        cardsInPlay[5] = hero[0];
        cardsInPlay[6] = hero[1];
        insertionSort(cardsInPlay, 7, cardsInPlayToScore);
        heroScore = findBestScore(cardsInPlayToScore);
        
        for (int j = 0; j < numOtherPlayers*2; j += 2) {
            cardsInPlay[5] = otherPlayerHands[j];
            cardsInPlay[6] = otherPlayerHands[j + 1];
            insertionSort(cardsInPlay, 7, cardsInPlayToScore);
            otherPlayerScore = findBestScore(cardsInPlayToScore);

            if (heroScore > otherPlayerScore) {
                wins += 1;
            } else if (heroScore < otherPlayerScore) {
                loses += 1;
            }
        }
    }

    winsLosesTies[0] = wins/(double)numRep;
    winsLosesTies[1] = loses/(double)numRep;
    winsLosesTies[2] = ((double)numRep - wins - loses)/(double)numRep;
} 

int main() {
    int hero[2] = {141, 142};
    double winsLoses[3] = { 0 };
    sim(1, hero, 1000000, winsLoses);
    printf("\nWins: %f\nLoses: %f\nTies: %f\n", winsLoses[0], winsLoses[1], winsLoses[2]);
    return 0;
}
