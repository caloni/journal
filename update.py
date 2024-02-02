import sys
import os
import glob
import imdb
import yaml
import urllib
import urllib2
import datetime
import PIL
from PIL import Image
import simplejson

ScreenshotBaseWidth = 800
GoogleImagesUrl = 'https://ajax.googleapis.com/ajax/services/search/images?'
ScreenshotsPath = r'C:\daytoday\cinetenisverde.github.io\images\screenshots'
PostsPath = r'C:\daytoday\cinetenisverde.github.io\_posts'
CoversPath = r'C:\daytoday\cinetenisverde.github.io\images\covers'
CoversDownloader = urllib.URLopener()
DefaultCoverSize = 95, 150
access = imdb.IMDb()
forceUpdate = len(sys.argv) > 1 and sys.argv[1] == '--force-update'
specificReview = len(sys.argv) > 1 and sys.argv[1].find('.md') != -1


def downloadscreenshots(query, fileName = None):
  url = GoogleImagesUrl + urllib.urlencode({ 'v': '1.0', 'q': query })
  print url
  request = urllib2.Request(url)
  response = urllib2.urlopen(request)
  results = simplejson.load(response)
  resultNumber = 0
  
  for i in results['responseData']['results']:
    try:
      Downloader = urllib.URLopener()
      path = fileName
      if path:
        path += str(resultNumber)
        resultNumber += 1
        if i['url'].find('.png') > 0:
          path += '.png'
        elif i['url'].find('.jpg') > 0:
          path += '.jpg'
      else:
        path = i['url'].split('/')[-1]
        ext = max(path.find('.png'), path.find('.jpg'))
        if ext > 0: path = path[:ext+4]

      print i['url'], ' >> ', path
      Downloader.retrieve(i['url'], path)
      img = Image.open(path)
      wpercent = (ScreenshotBaseWidth/float(img.size[0]))
      hsize = int((float(img.size[1])*float(wpercent)))
      img = img.resize((ScreenshotBaseWidth, hsize), Image.ANTIALIAS)
      img.save(path)
    except:
      pass


def loadreview(filePath):
  f = open(filePath)
  nextLine = f.readline()
  if nextLine == '---\n':
    header = ''
    nextLine = f.readline()
    while nextLine != '---\n':
      header = header + nextLine
      nextLine = f.readline()
    review = f.read()
    header = yaml.load(header)
    return header, review
  return None
    
def writereview(filePath, header, review, imdbData):

  def write(f, i, n):
    if isinstance(i, unicode):
        f.write(n + ': "' + i.encode('utf-8') + '"\n')
    else:
        f.write(n + ': "' + str(i) + '"\n')

  def writesimplearray(f, a, n, l = 10):
    l = 1
    f.write(n + ': [')
    for i in a:
      f.write('"' + i.encode('utf-8') + '", ')
      l = l + 1
      if l == l: break
    f.write(']\n')

  def writearray(f, a, n, l = 10):
    cl = 1
    f.write(n + ': [')
    for i in a:
      f.write('"' + i['name'].encode('utf-8') + '", ')
      cl = cl + 1
      if cl == l: break
    f.write(']\n')

  def writepersonarray(f, a, n, l = 10):
    cl = 1
    f.write(n + ': [')
    a2 = []
    for i in a:
      repeated = False
      for i2 in a2:
        if i.isSamePerson(i2) == 1:
          repeated = True
          break
      if repeated == False:
        a2.append(i)
        f.write('"' + i['name'].encode('utf-8') + '", ')
        cl = cl + 1
        if cl == l: break
    f.write(']\n')

  def writecastarray(f, a, n, l = 10):
    cl = 1
    f.write(n + ': [')
    for i in a:
      person = str(i)
      try:
        person = person.encode('utf-8')
      except UnicodeError:
        pass

      character = ''
      if isinstance(i.currentRole, list):
        character = ' / '.join([str(s) for s in i.currentRole])
      else:
        character = str(i.currentRole)
      try:
        character = character.encode('utf-8')
      except UnicodeError:
        pass

      if len(character) > 0:
        f.write('"' + person + ' (' + character + ')", ')
      else:
        f.write('"' + person + '", ')
      cl = cl + 1
      if cl == l: break
    f.write(']\n')

  f = open(filePath, 'w')
  f.write('---\n')
  for k, v in header.iteritems():
    if k[0] != '_':
      if isinstance(v, basestring):
        v = '"' + v.encode('utf-8') + '"'
      elif isinstance(v, datetime.datetime) or isinstance(v, datetime.date):
        v = v.isoformat()
      f.write(str(k) + ': ' + str(v) + '\n')
  if imdbData.has_key('title'): 
    write(f, imdbData['title'], '_title')
  if imdbData.has_key('year'): write(f, imdbData['year'], '_year')
  if imdbData.has_key('director'): writearray(f, imdbData['director'], '_director')
  if imdbData.has_key('writer'): writepersonarray(f, imdbData['writer'], '_writer')
  if imdbData.has_key('cast'): writecastarray(f, imdbData['cast'], '_cast')
  if imdbData.has_key('countries'): writesimplearray(f, imdbData['countries'], '_countries')
  if imdbData.has_key('languages'): writesimplearray(f, imdbData['languages'], '_languages')
  if imdbData.has_key('editor'): writearray(f, imdbData['editor'], '_editor')
  if imdbData.has_key('cinematographer'): writearray(f, imdbData['cinematographer'], '_cinematographer')
  if imdbData.has_key('original music'): writearray(f, imdbData['original music'], '_music')
  if imdbData.has_key('art direction'): writearray(f, imdbData['art direction'], '_artdirection')
  if imdbData.has_key('costume designer'): writearray(f, imdbData['costume designer'], '_costume designer')
  if imdbData.has_key('genres'): writesimplearray(f, imdbData['genres'], '_genres')
  if imdbData.has_key('runtimes'): writesimplearray(f, imdbData['runtimes'], '_runtimes')
  if imdbData.has_key('full-size cover url'): write(f, imdbData['full-size cover url'], '_fullcover')
  if imdbData.has_key('aspect ratio'): write(f, imdbData['aspect ratio'], '_ratio')
  if imdbData.has_key('kind'): write(f, imdbData['kind'], '_kind')
  if imdbData.has_key('cover url'): write(f, imdbData['cover url'], '_cover')
  access.update(imdbData, 'release dates')
  for rd in imdbData['release dates']:
    if rd.find('Brazil::') != -1:
        write(f, rd[rd.rfind(':')+1:], '_releasedate')
  f.write('---\n')
  f.write(review)
  f.close()

def updatereview(filePath):
  header, review = loadreview(filePath)
  if forceUpdate or not header.has_key('_year'):
    if header.has_key('imdb'):
      imdbId = int(header['imdb'])
      movieData = access.get_movie(imdbId)
      writereview(filePath, header, review, movieData)

def update(filePath, imdbId):
  resultFile = ''
  with open(filePath) as f:
    for line in f.readlines():
      if line.find('imdb:') == 0:
        resultFile = resultFile + 'imdb: "' + imdbId + '"\n'
      else:
        resultFile = resultFile + line
  with open(filePath, 'w') as f:
    f.write(resultFile)
  updatereview(filePath)

if __name__ == "__main__":
  if len(sys.argv) < 3:
    print 'Usage: update.py [file-path] [imdb-id]'
  else:
    update(sys.argv[1], sys.argv[2])

