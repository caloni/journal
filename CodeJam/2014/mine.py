import sys

def FieldToString(field):
        ret = '\n'
	for r in field:
		for c in r:
			ret = ret + str(c)
		ret = ret + '\n'
	return ret

def CountMines(field, r, c):
        ret = 0
        row = len(field)
        col = len(field[0])
        if r < row-1 and field[r+1][c] == '*': ret = ret + 1
        if c < col-1 and field[r][c+1] == '*': ret = ret + 1
        if r > 0 and field[r-1][c] == '*': ret = ret + 1
        if c > 0 and field[r][c-1] == '*': ret = ret + 1
        if r < row-1 and c < col-1 and field[r+1][c+1] == '*': ret = ret + 1
        if r > 0 and col > 0 and field[r-1][c-1] == '*': ret = ret + 1
        if r < row-1 and c > 0 and field[r+1][c-1] == '*': ret = ret + 1
        if r > 0 and c < col-1 and field[r-1][c+1] == '*': ret = ret + 1
        return ret

def ExpandClick(field, r, c):
        if field[r][c] != '0': return

        def Expand(field, r, c):
                if field[r][c] == '.':
                        field[r][c] = str(CountMines(field, r, c))
                        ExpandClick(field, r, c)

        row = len(field)
        col = len(field[0])
        if r < row-1 and field[r+1][c] != '*':
                Expand(field, r+1, c)
        if c < col-1 and field[r][c+1] != '*':
                Expand(field, r, c+1)
        if r > 0 and field[r-1][c] != '*':
                Expand(field, r-1, c)
        if c > 0 and field[r][c-1] != '*':
                Expand(field, r, c-1)
        if r < row-1 and c < col-1 and field[r+1][c+1] != '*':
                Expand(field, r+1, c+1)
        if r > 0 and col > 0 and field[r-1][c-1] != '*':
                Expand(field, r-1, c-1)
        if r < row-1 and c > 0 and field[r+1][c-1] != '*':
                Expand(field, r+1, c-1)
        if r > 0 and c < col-1 and field[r-1][c+1] != '*':
                Expand(field, r-1, c+1)

def FieldClicker(field):
	row = len(field)
	col = len(field[0])
        for r in range(row):
                for c in range(col):
                        if field[r][c] == 'C':
                                field[r][c] = str(CountMines(field, r, c))
                                ExpandClick(field, r, c)
                                break
        return field

def FieldValidate(field):
        ret = True
	row = len(field)
	col = len(field[0])
        for r in range(row):
                for c in range(col):
                        if field[r][c] == '.':
                                ret = False
                                break
        return ret

def FieldRender(row, col, mines):
	field = []
        for i in range(row):
                field.append(['.'] * col)
        if row == 2:
                nextRow = 0
                nextCol = 0
                while mines:
                        field[nextRow][nextCol] = '*'
                        nextRow = nextRow + 1
                        if nextRow == row:
                                nextRow = 0
                                nextCol = nextCol + 1
                        mines = mines - 1
                field[0][col-1] = 'C'
        elif col == 2:
                nextRow = 0
                nextCol = 0
                while mines:
                        field[nextRow][nextCol] = '*'
                        nextCol = nextCol + 1
                        if nextCol == col:
                                nextRow = nextRow + 1
                                nextCol = 0
                        mines = mines - 1
                field[row-1][0] = 'C'
	elif row * col - mines < 3:
                nextRow = row - 1
                nextCol = 0
                while mines:
                        field[nextRow][nextCol] = '*'
                        nextCol = nextCol + 1
                        if nextCol == col:
                                nextRow = nextRow - 1
                                nextCol = 0
                        mines = mines - 1
                field[0][col-1] = 'C'
        else:
                for r in range(len(field)):
                        for c in range(len(field[0])):
                                field[r][c] = '*'
                if row * col - mines >= 9 and row >= 3 and col >= 3:
                        empties = row * col - mines
                        nextRow = 0
                        nextCol = 0
                        while empties:
                                
	return field

def Mine(row, col, mines):
        if row * col - mines == 2 and row > 1 and col > 1:
		return 'Impossible!'
	if row * col - mines == 3:
		return 'Impossible!'
	elif row * col - mines == 5:
		return 'Impossible!'
	elif row * col - mines == 7:
		return 'Impossible!'
	else:
		return FieldToString(FieldRender(row, col, mines))

f = open(sys.argv[1])
total = int(f.readline())

for case in range(1, total + 1):
        field = [int(i) for i in f.readline().split()]
        print 'Case #' + str(case) + ': ' + Mine(field[0], field[1], field[2])



#############################################################################3

def FieldRenderWrong(row, col, mines):
	field = []
	for i in range(row):
		field.append(['.'] * col)

        def GetNextRow(field, clickRow, clickCol):
                row = len(field)
                col = len(field[0])
                nextRow = 0
                nextCol = 0
                rowDist = 0
                colDist = 0
                for r in range(row):
                        for c in range(col):
                                if field[r][c] == '.':
                                        rDist = abs(r - clickRow)
                                        cDist = abs(c - clickCol)
                                        totDist = rDist + cDist
                                        currTotDist = rowDist + colDist
                                        if totDist > currTotDist:
                                                nextRow = r
                                                rowDist = rDist
                                                nextCol = c
                                                colDist = cDist
                                        else:
                                                rowCount = 0
                                                for r2 in range(row):
                                                        if field[r2][c] == '*':
                                                                rowCount = rowCount + 1
                                                colCount = 0
                                                for c2 in range(col):
                                                        if field[r][c2] == '*':
                                                                colCount = colCount + 1
                                                lastRow = rowCount == row - 1
                                                lastCol = colCount == col - 1
                                                if lastRow or lastCol:
                                                        nextRow = r
                                                        rowDist = rDist
                                                        nextCol = c
                                                        colDist = cDist
                return nextRow, nextCol

        clickRow = 0
        clickCol = col-1
	field[clickRow][clickCol] = 'C'
        nextRow, nextCol = GetNextRow(field, clickRow, clickCol)
	while mines:
		field[nextRow][nextCol] = '*'
		nextRow, nextCol = GetNextRow(field, clickRow, clickCol)
		mines = mines - 1
	return field
