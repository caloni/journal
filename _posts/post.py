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

sys.path.append(r'c:\users\wanderley\.pwd')
import twitter_caloni as twitter_credentials
import facebook_caloni as facebook_credentials


baseUrl = 'http://www.caloni.com.br/' 


def GetShortener(shortener):
    ret = None
    if shortener == 'Google':
        ret = Shortener('Tinyurl')
        #todo get api key for caloni
        #ret = Shortener(shortener, api_key= 'AIzaSyCuDCcM1utV1zbkiRDd-TX_8FrYT9ApISw')
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


def PublishToTwitter(postInfo):
    """
    https://pypi.python.org/pypi/twitter
    """
    t = twitter.Twitter(auth=twitter_credentials.auth)
    
    with open(postInfo["screenshot"], "rb") as imagefile:
    	imagedata = imagefile.read()
    t_up = twitter.Twitter(domain='upload.twitter.com', auth=twitter_credentials.auth)
    id_img1 = t_up.media.upload(media=imagedata)["media_id_string"]
    st = postInfo['title'] + '\n' + postInfo['shortlink'].encode('utf-8')
    if len(st) > 120: # giving space to image attachment
        st = ' ' + postInfo['title'] + '\n\n' + '\n\n' + postInfo['shortlink'].encode('utf-8')
    t.statuses.update(status=st, media_ids=",".join([id_img1]))


def PublishToFacebook(postInfo):
    """
    http://nodotcom.org/python-facebook-tutorial.html
    """
    with open(postInfo["screenshot"], "rb") as imagefile:
    	imagedata = imagefile.read()

    st = postInfo['title'] + '\n\n' + postInfo['paragraph'] + '\n\n' + baseUrl + postInfo['permalink']
    post = facebook_credentials.auth.put_photo(image=imagedata, message=st)


def GetPostInfo(post):
    postInfo = { 'file' : post }
    postInfo['permalink'] =  re.match('[0-9]{4}-[0-9]{2}-[0-9]{2}-([^.]*).md', post).group(1)
    f = open(post)
    endHeader = 0
    firstParagraph = False
    for l in f.readlines():
        if endHeader == 2:
            m = re.search('^([^.?!]*.)', l)
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
    return postInfo


def PushChanges(postInfo):
    subprocess.call(['git', 'add', '..'])
    subprocess.call(['git', 'commit', '-m', 'One More Post'])
    subprocess.call(['git', 'push'])


def FindPostImageAndPrepare(postInfo):
    img = Image.open(postInfo['screenshot'])
    ScreenshotBaseWidth = 725
    wpercent = (ScreenshotBaseWidth/float(img.size[0]))
    hsize = int((float(img.size[1])*float(wpercent)))
    img = img.resize((ScreenshotBaseWidth, hsize), Image.ANTIALIAS)
    img.save(postInfo['screenshot'])

def PublishToSocialMedia(post):
    print 'Publishing ' + post + '...'
    afterMove = False
    try:
        print '*** Getting post info'
        postInfo = GetPostInfo(post)
        webbrowser.open_new_tab('https://www.google.com.br/search?q=' + postInfo['title'] + '&tbm=isch')
        subprocess.Popen('explorer "C:\\screenshots"')
        postInfo['screenshot'] = r'c:\screenshots\screenshot.jpg'
        print 'rename your screenshot image as c:\\screenshots\\screenshot.jpg and press any key to continue...'
        m.getch()
        print '*** Preparing image'
        FindPostImageAndPrepare(postInfo)
        print '*** Archiving file'
        shutil.move(post, 'archive')
        afterMove = True
        print '*** Pushing changes'
        PushChanges(postInfo)
        link = baseUrl + postInfo['permalink']
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

        webbrowser.open_new_tab(link)
        print 'press any key to continue and publish...'
        m.getch()
        # Testing image first
        with open(postInfo["screenshot"], "rb") as imagefile:
        	imagedata = imagefile.read()
        print '*** Publishing to Twitter'
        PublishToTwitter(postInfo)
        print '*** Publishing to Facebook'
        PublishToFacebook(postInfo)
        print '*** Done!'
        webbrowser.open_new_tab('https://www.facebook.com/bloguedocaloni/')
        webbrowser.open_new_tab('https://tweetdeck.twitter.com/')
    except Exception as e:
        print '*** Something gone wrong!'
        if afterMove == True:
            shutil.move('archive\\' + post, '.')
        raise

if __name__ == "__main__":
    for post in sys.argv[1:]:
        PublishToSocialMedia(post)
