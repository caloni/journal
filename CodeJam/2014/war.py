import sys

def BestBlock(block, blocks):
	bestBlock = 0
	for i in range(len(blocks) - 1, -1, -1):
		if blocks[i] < block: break
		bestBlock = blocks[i]
	if not bestBlock:
		bestBlock = blocks[0]
	return bestBlock

def War(naomi, ken):
	naomi = sorted(naomi)
	ken = sorted(ken)
	naomiCount = 0
	while len(naomi):
		naomiBlock = naomi[-1]
		kenBlock = BestBlock(naomiBlock, ken)
		if naomiBlock > kenBlock:
			naomiCount = naomiCount + 1
		#print str(naomiBlock) + ' vs ' + str(kenBlock) + ': ' + str(naomiCount)
		naomi.remove(naomiBlock)
		ken.remove(kenBlock)
	return naomiCount

def WarCheat(naomi, ken):
	naomi = sorted(naomi)
	ken = sorted(ken)
	naomiCount = 0
	while len(naomi):
		naomiTold = 0
		naomiBlock = naomi[-1]
		bestKen = ken[-1]
		if naomiBlock > bestKen:
			naomiTold = bestKen + 0.00000001
		else:
			naomiTold = bestKen - 0.00000001
		kenBlock = BestBlock(naomiTold, ken)
		naomiBlock = BestBlock(kenBlock, naomi)
		if naomiBlock > kenBlock:
			naomiCount = naomiCount + 1
		#print str(naomiTold) + '(' + str(naomiBlock) + ') vs ' + str(kenBlock) + ': ' + str(naomiCount)
		naomi.remove(naomiBlock)
		ken.remove(kenBlock)
	return naomiCount

f = open(sys.argv[1])
total = int(f.readline())

for case in range(1, total + 1):
        f.readline()
        naomi = [float(i) for i in f.readline().split()]
        ken = [float(i) for i in f.readline().split()]
        war = War(naomi, ken)
        warCheat = WarCheat(naomi, ken)
        print 'Case #' + str(case) + ': ' + str(warCheat)+ ' ' + str(war)
