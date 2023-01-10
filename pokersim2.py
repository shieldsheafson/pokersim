import random as rand

def hand_table(cards,num_hands):
    hand_and_table=rand.sample(cards,7+(num_hands-1)*2)
    hands=[]
    for num in range(4,6+(num_hands-1)*2):
        hands.append(hand_and_table.pop())
    return hand_and_table, hands
def prep_pool(real_pool):
    pool=real_pool.copy()
    duplicates=[0,0,0,0,0,0,0,0,0,0,0,0,0]
    to_remove=[]
    for num, card in enumerate(pool):
        pool[num] = int(card[:2])
        card=int(card[:2])
        if card==pool[num-1]: to_remove.append(card)
    for card in to_remove:
        duplicates[card-2]+=1
        pool.remove(card)
    return pool,duplicates
def high_card(pool):
    return (int(pool[-1][:2])*100_000_000)+(int(pool[-2][:2])*1_000_000)+(int(pool[-3][:2])*10_000)+(int(pool[-4][:2])*100)+(int(pool[-5][:2]))
def duplicate_hands(pool, duplicates): #pair, three of a kind, four of a kind, 2 pair, full house
    score=0
    pair=[]
    three=None
    four=None
    for num, item in enumerate(duplicates):
        if item == 1: pair.append(num+2)
        if item == 2: three=num+2
        if item == 3: four=num+2
    if bool(pair)==True: #blank_mod_pool is a list without the cards that are already in the hand
        pair_mod_pool = [int(x[:2]) for x in pool if pair[-1] != int(x[:2])]
        score=10_000_000_000+(pair[-1]*100_000_000)+(pair_mod_pool[-1]*1_000_000)+(pair_mod_pool[-2]*10_000)+(pair_mod_pool[-3]*100)
    if len(pair)>=2: #Two pair
        two_pair_mod_pool = [int(x[:2]) for x in pool if pair[-1] != int(x[:2]) and pair[-2] != int(x[:2])]
        score=20_000_000_000+(pair[-1]*100_000_000)+(pair[-2]*1_000_000)+(two_pair_mod_pool[-1]*10_000)
    if three!=None:
        three_mod_pool = [int(x[:2]) for x in pool if three != int(x[:2])]
        score=30_000_000_000+(three*100_000_000)+(three_mod_pool[-1]*1_000_000)+(three_mod_pool[-2]*10_000)
    if bool(three)==True and bool(pair)==True: score=60_000_000_000+(three*100_000_000)+(pair[-1]*1_000_000) #Full House
    if four!=None:
        four_mod_pool = [int(x[:2]) for x in pool if four != int(x[:2])]
        score=70_000_000_000+(four*100_000_000)+(four_mod_pool[-1]*1_000_000)
    if score!=0: return score
def straight(pool):
    straight=[]
    if len(pool)>=5:
        if pool[-1]==14 and pool[:4]==[2,3,4,5]: straight=[1,2,3,4,5] #special bit for aces
        for num in range(len(pool)-4):
            if sum(pool[num:num+5])==(pool[num+4]-pool[num]+1)*(pool[num] + pool[num+4])/2: straight=pool[num:num+5]
    if straight: return 40_000_000_000+straight[-1]*100_000_000
def flush(pool): #and straight flush
    score=0
    score_two=0
    suits={'s':[],'c':[],'d':[],'h':[]}
    for card in pool:
        suits[card[-1]].append(card)
        if len(suits[card[-1]])>=5:
            score=50_000_000_000+(int(suits[card[-1]][-1][:2])*100_000_000)+(int(suits[card[-1]][-2][:2])*1_000_000)+(int(suits[card[-1]][-3][:2])*10_000)+(int(suits[card[-1]][-4][:2])*100)+(int(suits[card[-1]][-5][:2]))
            if card[:2]=='14' and 14==int(suits[card[-1]][0][:2])+int(suits[card[-1]][1][:2])+int(suits[card[-1]][2][:2])+int(suits[card[-1]][3][:2]) and score_two==0:
                score_two=80_500_000_000
            if int(suits[card[-1]][-1][:2])+int(suits[card[-1]][-2][:2])+int(suits[card[-1]][-3][:2])+int(suits[card[-1]][-4][:2])+int(suits[card[-1]][-5][:2])==5*(int(suits[card[-1]][-5][:2])+int(suits[card[-1]][-1][:2]))/2:
                score_two=80_000_000_000+int(suits[card[-1]][-1][:2])*100_000_000
    if score_two: return score_two
    elif score: return score
def scorer(pool): #returns highest score
    pool.sort()
    prepped_pool, duplicates=prep_pool(pool)
    scores=[high_card(pool),duplicate_hands(pool,duplicates),flush(pool),straight(prepped_pool)]
    scores.sort(key=lambda e: (e is not None, e))
    return scores[-1]
def sim(num_reps, hand): # hand is a string with no spaces: cardnum cardnum o/s
    score=[0,0]
    cards = ['02-s', '03-s', '04-s', '05-s', '06-s', '07-s', '08-s', '09-s', '10-s','11-s', '12-s', '13-s', '14-s', '02-c', '03-c', '04-c', '05-c', '06-c','07-c', '08-c', '09-c', '10-c', '11-c', '12-c', '13-c', '14-c', '02-d','03-d', '04-d', '05-d', '06-d', '07-d', '08-d', '09-d', '10-d', '11-d','12-d', '13-d', '14-d', '02-h', '03-h', '04-h', '05-h', '06-h', '07-h','08-h', '09-h', '10-h', '11-h', '12-h', '13-h', '14-h']
    if hand[-1]=='o': hand = [hand[:2] + '-s',hand[2:4] + '-d']
    else:  hand = [hand[:2] + '-s',hand[2:4] + '-s']
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
        if hand_score>other_hand_score: score[0]+=1
        elif hand_score<other_hand_score: score[1]+=1
    return(score, 'win % = ' + str((score[0] /num_reps) * 100) + '%', 'win and tie % = ' + str(((score[0] + num_reps-score[0]-score[1]) / num_reps) * 100) + '%')

def sim_two(num_reps, hand): #used for testing, not important
    score=[0,0,0]
    cards = ['02-s', '03-s', '04-s', '05-s', '06-s', '07-s', '08-s', '09-s', '10-s','11-s', '12-s', '13-s', '14-s', '02-c', '03-c', '04-c', '05-c', '06-c','07-c', '08-c', '09-c', '10-c', '11-c', '12-c', '13-c', '14-c', '02-d','03-d', '04-d', '05-d', '06-d', '07-d', '08-d', '09-d', '10-d', '11-d','12-d', '13-d', '14-d', '02-h', '03-h', '04-h', '05-h', '06-h', '07-h','08-h', '09-h', '10-h', '11-h', '12-h', '13-h', '14-h']
    if hand[-1]=='o': hand = [hand[:2] + '-s',hand[2:4] + '-d']
    else:  hand = [hand[:2] + '-s',hand[2:4] + '-s']
    cards.remove(hand[0])
    cards.remove(hand[1])
    deck=cards.copy()
    for rep in range(num_reps):
        current_table = table(cards)
        current_table_two=current_table.copy()
        other_hand = hands(1, cards)['hand1']
        #print(current_table)
        current_table.extend(hand)
        hand_score = scorer(current_table)
        #print(current_table)
        current_table_two.extend(other_hand)
        other_hand_score = scorer(current_table_two)
        #print(current_table_two)
        #print(str(hand)+' score '+str(hand_score),'\n'+str(other_hand)+' score '+str(other_hand_score))
        if hand_score>other_hand_score:
            score[0]+=1
            print('win')
        elif hand_score<other_hand_score:
            score[1]+=1
            print('lose')
        else:
            score[2]+=1
            print('tie')
        cards=deck.copy()
        print('''
''')
    return(score, 'win % = ' + str((score[0] /(score[0] + score[1] + score[2])) * 100) + '%', 'win and tie % = ' + str(((score[0] + score[2]) / (score[0] + score[1] + score[2])) * 100) + '%')

##cards = ['02-s', '03-s', '04-s', '05-s', '06-s', '07-s', '08-s', '09-s', '10-s','11-s', '12-s', '13-s', '14-s', '02-c', '03-c', '04-c', '05-c', '06-c','07-c', '08-c', '09-c', '10-c', '11-c', '12-c', '13-c', '14-c', '02-d','03-d', '04-d', '05-d', '06-d', '07-d', '08-d', '09-d', '10-d', '11-d','12-d', '13-d', '14-d', '02-h', '03-h', '04-h', '05-h', '06-h', '07-h','08-h', '09-h', '10-h', '11-h', '12-h', '13-h', '14-h']
##print(hand_table(cards,10))

print(sim(10_000_000,'0503o'))
