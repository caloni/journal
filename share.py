# coding=utf8
import twitter
import sys
import re
import shutil
import subprocess
import urllib2
import time
import glob
from PIL import Image
import os
import webbrowser
import msvcrt as m
from pyshorteners import Shortener
import frontmatter
from bs4 import BeautifulSoup
from markdown import markdown

sys.path.append(r'c:\users\caloni\.pwd')
import twitter_cinetenisverde as twitter_credentials
import facebook_cinetenisverde as facebook_credentials
import adorocinema_cinetenisverde as adorocinema_credentials


baseUrl = 'http://www.cinetenisverde.com.br/' 



def WebPageExists(url):
    try:
        urllib2.urlopen(url)
    except urllib2.HTTPError as e:
        return False
    except urllib2.URLError as e:
        return False
    return True


def PrintStars(stars):
    ret = ''
    starFull = unichr(0x2605)
    starEmpty = unichr(0x2606)
    for s in range(0, int(stars)):
        ret = ret + starFull
    for s in range(int(stars), 5):
        ret = ret + starEmpty
    ret = ret + ' '
    return ret.encode('utf-8')


def GetSentence(postInfo, remaining):
    sentence = postInfo['paragraph']
    sentenceEnd = sentence.find('.')
    if sentenceEnd == -1:
        sentenceEnd = min(remaining, len(s))
    sentenceEnd = min(sentenceEnd, remaining)
    sentence = sentence[0:sentenceEnd+1]
    if sentence[-1] == ' ':
        sentence = sentence[0:-1] + '...'
    elif sentence[-1] != '.':
        sentence = sentence + '...'
    return sentence.decode('utf8')


def PublishToTwitter(postInfo, img):
    """
    https://pypi.python.org/pypi/twitter
    """
    t = twitter.Twitter(auth=twitter_credentials.auth)
    
    t_up = twitter.Twitter(domain='upload.twitter.com', auth=twitter_credentials.auth)
    id_img1 = t_up.media.upload(media=img)["media_id_string"]

    stars = PrintStars(postInfo['stars']) if postInfo.has_key('stars') else ''
    remaining = 130 - len(stars.decode('utf8') + ' ' + postInfo['subtitle'].decode('utf8') + '\n' + '\n' + postInfo['shortlink'])
    st = stars.decode('utf8') + ' ' + postInfo['subtitle'].decode('utf8') + '\n' + postInfo['shortlink']
    t.statuses.update(status=st, media_ids=",".join([id_img1]))


def MakeSharePost(postInfo):
    st = postInfo['subtitle'].decode('utf8') + '\n\n' + postInfo['desc'].decode('utf8') + '\n\n' + postInfo['paragraph'].decode('utf8') + '\n\n' + postInfo['shortlink'].decode('utf8')
    if postInfo.has_key('cabine'):
        st = st + '\n\n' + 'Em breve crítica completa no www.cinemaqui.com.br.'.decode('utf8')
    return st

def PublishToFacebook(st, img):
    """
    http://nodotcom.org/python-facebook-tutorial.html
    """
    stars = PrintStars(postInfo['stars']) if postInfo.has_key('stars') else ''
    st = stars.decode('utf8') + ' ' + st
    post = facebook_credentials.auth.put_photo(image=img, message=st)

def SearchAdoroCinema(postInfo):
    titleSearch = postInfo['title'].decode('utf8')
    titleSearch = titleSearch.replace(' ', '+')
    webbrowser.open_new_tab('http://www.adorocinema.com/busca/?q=' + titleSearch)


def TreatParagraph(p):
    p = p.decode('utf8')
    p = unicode(p)
    html = markdown(p)
    text = ''.join(BeautifulSoup(html, 'lxml').findAll(text=True))
    text = text.encode('utf8')
    return text


def GetPostInfo(post):
    postInfo = { 'file' : 'content\\post\\' + post + '.md' }
    postInfo['permalink'] =  post
    postInfo['category'] = 'movies'
    f = open(postInfo['file'])
    endHeader = 0
    firstParagraph = False
    for l in f.readlines():
        if endHeader == 2:
            m = re.search('^([^.]*.)', l)
            if m:
                postInfo['tagline'] = m.group(1)
                postInfo['paragraph'] = TreatParagraph(l)
                endHeader = 0
        if l == '---\n':
            endHeader += 1
            pass
        m = re.match('^title: \"(.*)\"', l)
        if m:
            postInfo['title'] = m.group(1)
        m = re.match('^stars: \"([1-5])/5\"', l)
        if m:
            postInfo['stars'] = m.group(1)
        m = re.match('^category: \"(.*)\"', l)
        if m:
            postInfo['category'] = m.group(1)
        m = re.match('^cabine: \"(.*)\"', l)
        if m:
            postInfo['cabine'] = True
        m = re.match('^desc: \"(.*)\"', l)
        if m:
            postInfo['desc'] = m.group(1)
    return postInfo


def PushChanges(postInfo):
    subprocess.call(['git', 'add', postInfo['category'] + '/' + postInfo['file']])
    subprocess.call(['git', 'add', '..\\images\\screenshots\\' + postInfo['permalink'] + '.jpg'])
    #todo: fix encoding thing
    #subprocess.call(['git', 'commit', '-m', '\"' + postInfo['tagline'].encode(sys.stdout.encoding) + '\"'])
    subprocess.call(['git', 'commit', '-m', 'Movie Review'])
    subprocess.call(['git', 'push'])


def FindPostImageAndPrepare(postInfo):
    ScreenshotBaseWidth = 725
    imgs = glob.glob('*.jpg')
    if len(imgs) > 0 :
        origPath = imgs[0]
        newPath = postInfo['permalink'] + '.jpg'
        img = Image.open(origPath)
        wpercent = (ScreenshotBaseWidth/float(img.size[0]))
        hsize = int((float(img.size[1])*float(wpercent)))
        img = img.resize((ScreenshotBaseWidth, hsize), Image.ANTIALIAS)
        img.save(newPath)
        if origPath != newPath:
            os.remove(origPath)

def PublishToSocialMedia(comment, post, img):
    print 'Publishing ' + post + '...'
    afterMove = False
    republish = False
    try:
        print '*** Getting post info'
        postInfo = GetPostInfo(post)
        link = baseUrl + postInfo['permalink']

        print '*** Waiting page ' + link
        while WebPageExists(link) == False:
            time.sleep(10)

        postInfo['shortlink'] = baseUrl + postInfo['permalink']
        comment = comment.decode(sys.stdin.encoding)
        postInfo['subtitle'] = comment.encode('utf8')
        try:
            shortener = Shortener('Google', api_key= 'AIzaSyCuDCcM1utV1zbkiRDd-TX_8FrYT9ApISw')
            postInfo['shortlink'] = shortener.short(postInfo['shortlink'])
        except Exception as e:
            print "Exception in shortener, using full url ", postInfo['shortlink']

        imgUrl = urllib2.urlopen(img)
        img = imgUrl.read()
        st = MakeSharePost(postInfo)
        print '========== cut here =========='
        print st
        print '========== cut here =========='
        print '*** Publishing to Twitter'
        PublishToTwitter(postInfo, img)
        print '*** Publishing to Facebook'
        PublishToFacebook(postInfo, st, img)
        print '*** Done!'
        SearchAdoroCinema(postInfo)
        webbrowser.open_new_tab(link)
        webbrowser.open_new_tab('https://plus.google.com/b/116314610297829036822')
    except Exception as e:
        print '*** Something gone wrong!'
        raise

if len(sys.argv) < 4:
    print 'How to use: share.py comment post-slug http://image-link'
else:
    PublishToSocialMedia(sys.argv[1], sys.argv[2], sys.argv[3])


