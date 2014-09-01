import os
import csv

def ExportReviewToJekyll(review):
	origFile = open('reviews\\' + review['id'] + '.md')
	destFile = open('jekyll\\' + review['first-date'] + '-' + review['id'] + '.md', 'w')
	destFile.write('---\n')
	destFile.write('title: "' + review['title'].decode('latin-1').encode('utf-8') + '"\n')
	destFile.write('imdb: "' + review['imdb'][2:] + '"\n')
	if review['first-date'] != review['date']:
		destFile.write('date: ' + review['date'] + '\n')
	if review['count'] != '1':
		destFile.write('count: ' + review['count'] + '\n')
	destFile.write('stars: ' + review['ranking'] + '/5\n')
	destFile.write('---\n')
	destFile.write(origFile.read().decode('latin-1').encode('utf-8'))
	destFile.close()
	origFile.close()

csvPath = r'lists\index.csv'
csvFile = csv.DictReader(open(csvPath, 'rb'), delimiter=',')
reviews = []
for line in csvFile:
    reviews.append(line)
if not os.path.exists('jekyll'):
    os.mkdir('jekyll')
for review in reviews:
	ExportReviewToJekyll(review)
