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

sys.path.append(r'c:\users\wanderley\.pwd')
import twitter_cinetenisverde as twitter_credentials
import facebook_cinetenisverde as facebook_credentials


baseUrl = 'http://www.cinetenisverde.com.br/' 


def GetShortener(shortener):
    ret = None
    if shortener == 'Google':
        ret = Shortener(shortener, api_key= 'AIzaSyCuDCcM1utV1zbkiRDd-TX_8FrYT9ApISw')
    else:
        ret = Shortener(shortener)
    return ret


def WebPageExists(url):
    try:
        urllib2.urlopen(url)
    except urllib2.HTTPError, e:
        return False
    except urllib2.URLError, e:
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


def PublishToTwitter(postInfo):
    """
    https://pypi.python.org/pypi/twitter
    """
    t = twitter.Twitter(auth=twitter_credentials.auth)
    
    with open("C:\\projects\\cinetenisverde.github.io\\images\\screenshots\\" + postInfo["permalink"] + ".jpg", "rb") as imagefile:
    	imagedata = imagefile.read()
    t_up = twitter.Twitter(domain='upload.twitter.com', auth=twitter_credentials.auth)
    id_img1 = t_up.media.upload(media=imagedata)["media_id_string"]
    stars = PrintStars(postInfo['stars']) if postInfo.has_key('stars') else ''
    st = stars + ' ' + postInfo['title'] + ' ' + postInfo['shortlink'].encode('utf-8') + postInfo['tags']
    t.statuses.update(status=st, media_ids=",".join([id_img1]))


def PublishToFacebook(postInfo):
    """
    http://nodotcom.org/python-facebook-tutorial.html
    """
    with open("C:\\projects\\cinetenisverde.github.io\\images\\screenshots\\" + postInfo["permalink"] + ".jpg", "rb") as imagefile:
    	imagedata = imagefile.read()

    stars = PrintStars(postInfo['stars']) if postInfo.has_key('stars') else ''
    st = stars + ' ' + postInfo['title'] + '\n\n' + postInfo['paragraph'] + '\n\n' + 'http://www.cinetenisverde.com.br/' + postInfo['permalink'] + postInfo['tags']
    post = facebook_credentials.auth.put_photo(image=imagedata, message=st)


def GetPostInfo(post):
    postInfo = { 'file' : post }
    postInfo['permalink'] =  re.match('[0-9]{4}-[0-9]{2}-[0-9]{2}-([^.]*).md', post).group(1)
    postInfo['category'] = 'movies'
    f = open(post)
    endHeader = 0
    firstParagraph = False
    for l in f.readlines():
        if endHeader == 2:
            m = re.search('^([^.]*.)', l)
            if m:
                postInfo['tagline'] = m.group(1)
                postInfo['paragraph'] = l
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
    postInfo['tags'] = ''
    with open(post) as f:
        metadata, content = frontmatter.parse(f.read())
        if metadata.has_key('tags'):
            for t in metadata['tags']:
                postInfo['tags'] = postInfo['tags'] + ' #' + t
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

def PublishToSocialMedia(post):
    print 'Publishing ' + post + '...'
    afterMove = False
    republish = False
    try:
        print '*** Getting post info'
        postInfo = GetPostInfo(post)
        if os.path.isfile('..\\images\\screenshots\\' + postInfo['permalink'] + '.jpg'):
            print '*** Existig image archived!'
            shutil.move('..\\images\\screenshots\\' + postInfo['permalink'] + '.jpg', '.')
            republish = True
        else:
            subprocess.Popen('explorer "C:\\projects\\cinetenisverde.github.io\\_posts"')
        webbrowser.open_new_tab('https://www.google.com.br/search?q=' + postInfo['title'] + '&tbm=isch')
        print 'press any key to continue...'
        m.getch()
        print '*** Preparing image'
        FindPostImageAndPrepare(postInfo)
        print '*** Moving files'
        shutil.move(postInfo['permalink'] + '.jpg', r'..\images\screenshots')
        shutil.move(post, postInfo['category'])
        afterMove = True
        print '*** Pushing changes'
        PushChanges(postInfo)
        link = baseUrl + postInfo['permalink']

        if republish == True:
            webbrowser.open_new_tab(link)
            print 'press any key to continue and publish...'
            m.getch()
        else:
            print '*** Waiting page ' + link
            while WebPageExists(link) == False:
                time.sleep(10)

        lastShortener = 'Google'
        shortenerOk = False
        while shortenerOk == False:
            time.sleep(3)
            try:
                shortener = GetShortener(lastShortener)
                postInfo['shortlink'] = shortener.short(baseUrl + postInfo['permalink']).encode('utf-8')
                shortenerOk = True
            except Exception as e:
                print "Exception in shortener, waiting: ", str(e)
                lastShortener = 'Google' if lastShortener != 'Google' else 'Tinyurl'

        print '*** Publishing to Twitter'
        PublishToTwitter(postInfo)
        print '*** Publishing to Facebook'
        PublishToFacebook(postInfo)
        print '*** Done!'
        if republish == False:
            webbrowser.open_new_tab(link)
        webbrowser.open_new_tab('https://www.facebook.com/cinetenisverde/')
        webbrowser.open_new_tab('https://twitter.com/')
    except Exception as e:
        print '*** Something gone wrong!'
        if afterMove == True:
            shutil.move('..\\images\\screenshots\\' + postInfo['permalink'] + '.jpg', '.')
            shutil.move(postInfo['category'] + '\\' + post, '.')
        raise

for post in sys.argv[1:]:
    PublishToSocialMedia(post)

