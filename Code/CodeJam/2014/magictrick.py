import sys

f = open(sys.argv[1])
total = int(f.readline())

for case in range(0, total):
	guess1 = int(f.readline())
	row1 = None
	for i in range(1, 5):
		row = f.readline().split()
		if i == guess1:
			row1 = row
	guess2 = int(f.readline())
	row2 = None
	for i in range(1, 5):
		row = f.readline().split()
		if i == guess2:
			row2 = row
	cards = list(set(row1) & set(row2))
	if len(cards) == 1:
		print 'Case #' + str(case+1) + ': ' + cards[0]
	elif len(cards) > 1:
		print 'Case #' + str(case+1) + ': Bad magician!'
	else:
		print 'Case #' + str(case+1) + ': Volunteer cheated!'
