# Solving 2048 game and generating images.
# author Wanderley Caloni <wanderley.caloni@gmail.com>
# date 2020-08

from PIL import Image
from PIL import ImageFont
from PIL import ImageDraw
from itertools import islice

TilePositions = [
        [32, 216], [153, 216], [274, 216], [395, 216],
        [32, 339], [153, 339], [274, 339], [395, 339],
        [32, 461], [153, 461], [274, 461], [395, 461],
        [32, 584], [153, 584], [274, 584], [395, 584]
	]
TileSize = 106

def LoadTemplate():
	img = Image.open('2048.png')
	draw= ImageDraw.Draw(img)
	return img, draw

def DrawBoard(board):
	img, draw = LoadTemplate()
	bigFont = ImageFont.truetype('DejaVuSans-Bold.ttf', 48)
	smallFont = ImageFont.truetype('DejaVuSans-Bold.ttf', 24)
	dbgFont = ImageFont.truetype('DejaVuSans-Bold.ttf', 8)
	for r in range(len(board)):
		for c in range(len(board[r])):
			number = 2 ** board[r][c]
			if number > 1:
				tileX, tileY = TilePositions[r*4 +c][0], TilePositions[r*4 +c][1]
				font = smallFont if number > 64 else bigFont
				text = str(number)
				textW, textH = draw.textsize(text, font=font)
				x, y = tileX + (TileSize - textW) / 2, tileY + (TileSize - textH) /2
				dbgText = str(tileX) + ' + (' + str(TileSize) + ' - ' + str(textW) + ') / 2'
                                draw.text((x, y), text, (0,0,0), font=font)
	return img

def ReadNextBoard(f):
	lines = [lines for lines in islice(f, 4)]
	if (len(lines)) == 4:
		board = [
			[ int(lines[0][0], 16), int(lines[0][1], 16), int(lines[0][2], 16), int(lines[0][3], 16) ],
			[ int(lines[1][0], 16), int(lines[1][1], 16), int(lines[1][2], 16), int(lines[1][3], 16) ],
			[ int(lines[2][0], 16), int(lines[2][1], 16), int(lines[2][2], 16), int(lines[2][3], 16) ],
			[ int(lines[3][0], 16), int(lines[3][1], 16), int(lines[3][2], 16), int(lines[3][3], 16) ]
			]
		return board
	return None

def ProcessGame(filePath):
    f = open(filePath)
    board = ReadNextBoard(f)
    boardNumber = 1
    while board:
            DrawBoard(board).save('2048-' + str(boardNumber) + '.png')
            board = ReadNextBoard(f)
            boardNumber = boardNumber + 1

