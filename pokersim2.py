import random as rand

def hand_table(cards, num_hands): 
    
    # returns a list of 5 cards, and
    # a list of cards length num_hands * 2
    
    hand_and_table = rand.sample(cards, 7 + (num_hands - 1) * 2)
    hands = []
    
    for num in range(4, 6 + (num_hands - 1) * 2): 
        hands.append(hand_and_table.pop())
    
    return hand_and_table, hands

def remove_all(list, item):
 
    # uses list comprehension to 
    # remove all instances of an item from a list

    # returns a list

    list_with_items_removed = [x for x in list if x!=item]
 
    return list_with_items_removed

def prep_pool(real_pool): 
    
    # creates 3 lists:
    # one with just the values of cards
    # one with just the values and no duplicates
    # one with a list of all the duplicates in the pool

    # returns 3 lists
    
    pool = real_pool.copy()
    duplicates = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    straight_pool = []
    temp_pool = pool.copy()
    pool.clear()
    
    for num,card in enumerate(temp_pool):
        card = int(card / 10000)
        pool.append(card)
        if num != 0 and pool[num-1] == card: duplicates[card - 2] += 1
        else: straight_pool.append(card)
    
    return pool, straight_pool, duplicates

# each hand function returns a score that ranges from 605_040_302 to 81_400_000_000
# the first digit represents the hand
# and the rest of the digits are the deciding cards in the hand

# each hand function returns an integer
 
def high_card(pool):
    
    return (pool[-1] * 100_000_000 
            + pool[-2] * 1_000_000 
            + pool[-3] * 10_000 
            + pool[-4] * 100
            + pool[-5])

def duplicate_hands(pool, duplicates): 
    
    # the function for scoring pair, three of a kind, four of a kind, 2 pair, full house
    
    score = 0
    pair = [] # is a list for two pair
    three = None
    four = None
    
    # uses the "duplicates" to find the highest pairs
    # three of kinds, and four of kinds
    # the + 2 is because the program uses list indexing to find 
    # the value of the duplicate, and the card values start at 2
    
    for num, item in enumerate(duplicates):
        if item == 1: 
            pair.append(num + 2)
        if item == 2: 
            three = num + 2
        if item == 3: 
            four = num + 2
    
    if four:
        four_mod_pool = remove_all(pool, four)
        score = (70_000_000_000 
                 + four * 100_000_000 
                 + four_mod_pool[-1] * 1_000_000)

    elif three and pair:  #Full House
        score = (60_000_000_000 
                 + three * 100_000_000
                 + pair[-1] * 1_000_000)
    
    elif three:
        three_mod_pool = remove_all(pool, three)
        score = (30_000_000_000 
                 + three * 100_000_000 
                 + three_mod_pool[-1] * 1_000_000 
                 + three_mod_pool[-2] * 10_000)

    elif len(pair)>=2:  #Two pair
        two_pair_mod_pool = [x for x in pool 
                             if pair[-1] != x and pair[-2] != x]
        score = (20_000_000_000
                 + pair[-1] * 100_000_000 
                 + pair[-2] * 1_000_000 
                 + two_pair_mod_pool[-1] * 10_000)

    elif pair: 
        pair_mod_pool = remove_all(pool, pair[-1])
        score = (10_000_000_000 
                 + pair[-1] * 100_000_000 
                 + pair_mod_pool[-1] * 1_000_000 
                 + pair_mod_pool[-2] * 10_000 
                 + pair_mod_pool[-3] * 100)
    
    return score

def straight(pool): 
    
    # is passed list of cards as only values and without duplicates

    score = 0
    
    #checks to make sure that there are at least five cards

    if len(pool) >= 5:
        
        # checks if there is a straight starting with an ace 
        # as the starting card

        if pool[-1] == 14 and pool[:4] == [2, 3, 4, 5]: 
            score = 40_500_000_000
        
        # loops through the cards between 1 and 3 times
        # depending on whether there are 5, 6, or 7 cards

        for num in range(len(pool)-4):
            
            # checks if the sum of a set a five cards
            # is equal to the sum of the integers between
            # the lowest and highest cards in the 5 cards
            # if so there must be a straight
            if (sum(pool[num: num + 5]) == 
                (pool[num + 4] - pool[num] + 1) 
                * (pool[num] + pool[num + 4]) 
                / 2):

                score = 40_000_000_000 + pool[num + 4] * 100_000_000
    
    return score

def flush(pool):  #and straight flush
    
    score = 0
    score_two = 0

    sum_of_suits = str(sum(pool))[-4:]
    
    #checks if flush is in the pool of cards

    if '5' in sum_of_suits or '6' in sum_of_suits or '7' in sum_of_suits:
        
        suits={'s':[],'c':[],'d':[],'h':[]}
        
        for card in pool:
            
            # adding cards to the dictionary of suits

            card = str(card).zfill(6)
            if card[-1] == '1': 
                card = card[:2] + '-s'
            elif card[-2] == '1': 
                card = card[:2] + '-c'
            elif card[-3] == '1': 
                card = card[:2] + '-d'
            else: 
                card = card[:2] + '-h'
            suits[card[-1]].append(card)
            
            # finds cards are part of the flush

            if len(suits[card[-1]]) >= 5:
                
                # creates score for flush
                score = (50_000_000_000 
                         + int(suits[card[-1]][-1][:2]) * 100_000_000
                         + int(suits[card[-1]][-2][:2]) * 1_000_000 
                         + int(suits[card[-1]][-3][:2]) * 10_000 
                         + int(suits[card[-1]][-4][:2]) * 100
                         + int(suits[card[-1]][-5][:2]))
                
                # checks for straight flush

                # checks if straight flush starting with an ace
                if (card[:2] == '14' 
                    and 14 == (int(suits[card[-1]][0][:2]) 
                               + int(suits[card[-1]][1][:2]) 
                               + int(suits[card[-1]][2][:2]) 
                               + int(suits[card[-1]][3][:2])) 
                    and score_two==0):
                    score_two = 80_500_000_000
                
                # checks for other straight flushes using
                # similar method to the one in "straight"

                if (int(suits[card[-1]][-1][:2]) 
                    + int(suits[card[-1]][-2][:2]) 
                    + int(suits[card[-1]][-3][:2]) 
                    + int(suits[card[-1]][-4][:2]) 
                    + int(suits[card[-1]][-5][:2]) 
                    == 
                    5 * (int(suits[card[-1]][-5][:2]) 
                    + int(suits[card[-1]][-1][:2]))
                    / 2):
                    
                    score_two = (80_000_000_000 
                                 + int(suits[card[-1]][-1][:2]) * 100_000_000)
    
    if score_two: 
        return score_two
    else: 
        return score

def scorer(pool):
    
    # finds highest score

    # returns integer
    
    pool.sort()
    prepped_pool, straight_pool, duplicates = prep_pool(pool)
    
    scores = [high_card(prepped_pool), 
              duplicate_hands(prepped_pool,duplicates), 
              flush(pool), 
              straight(straight_pool)]

    scores.sort() 
    
    return scores[-1]

def sim(num_reps, hand): # hand is a string with no spaces: cardnum cardnum o/s
    
    score=[0,0]
  
    cards = [20001, 30001, 40001, 50001, 60001, 70001, 80001, 90001, 100001, 110001, 120001, 130001, 140001, 
             20010, 30010, 40010, 50010, 60010, 70010, 80010, 90010, 100010, 110010, 120010, 130010, 140010, 
             20100, 30100, 40100, 50100, 60100, 70100, 80100, 90100, 100100, 110100, 120100, 130100, 140100, 
             21000, 31000, 41000, 51000, 61000, 71000, 81000, 91000, 101000, 111000, 121000, 131000, 141000]
    
    # creates hand the user requested and removes those cards
    # from the list of cards

    if hand[-1]=='o': 
        hand = [int(hand[:2] + '0001'),int(hand[2:4] + '0010')]
    else:  
        hand = [int(hand[:2] + '0001'),int(hand[2:4] + '0001')]
    cards.remove(hand[0])
    cards.remove(hand[1])
    
    for rep in range(num_reps):
        
        table,hands=hand_table(cards,1)
        table_copy=table.copy()
        other_hand = hands[-2:]
        table.extend(hand)
        hand_score = scorer(table)
        table_copy.extend(other_hand)
        other_hand_score = scorer(table_copy)
        
        if hand_score > other_hand_score: 
            score[0]+=1
        elif hand_score < other_hand_score: 
            score[1]+=1
    
    return (score, 
           'win % = ' + str((score[0] /num_reps) * 100) + '%', 
           'win and tie % = ' + str(((score[0] + num_reps-score[0]-score[1]) / num_reps) * 100) + '%')

sim(10_000_000,'0403o')
