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
from selenium import webdriver
from selenium.webdriver.common.keys import Keys

sys.path.append(r'c:\users\caloni\.pwd')
import twitter_cinetenisverde as twitter_credentials
import facebook_cinetenisverde as facebook_credentials
import adorocinema_cinetenisverde as adorocinema_credentials


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


def PublishToTwitter(postInfo, img):
    """
    https://pypi.python.org/pypi/twitter
    """
    t = twitter.Twitter(auth=twitter_credentials.auth)
    
    t_up = twitter.Twitter(domain='upload.twitter.com', auth=twitter_credentials.auth)
    id_img1 = t_up.media.upload(media=img)["media_id_string"]
    stars = PrintStars(postInfo['stars']) if postInfo.has_key('stars') else ''
    st = stars + ' ' + postInfo['title'] + ' ' + postInfo['shortlink'] + ' ' + postInfo['tags']
    t.statuses.update(status=st, media_ids=",".join([id_img1]))


def PublishToFacebook(postInfo, img):
    """
    http://nodotcom.org/python-facebook-tutorial.html
    """
    stars = PrintStars(postInfo['stars']) if postInfo.has_key('stars') else ''
    st = stars + ' ' + postInfo['title'] + '\n\n' + postInfo['paragraph'] + '\n\n' + 'http://www.cinetenisverde.com.br/' + postInfo['permalink'] + '\n' + postInfo['tags']
    post = facebook_credentials.auth.put_photo(image=img, message=st)


def PublishToAdoroCinema(postInfo):
    driver = webdriver.Chrome()
    postUrl = 'http://www.cinetenisverde.com.br/' + postInfo['permalink']
    adoroCinemaUrl = 'http://www.adorocinema.com/comunidade/filmes/filme-' + postInfo['adoroCinemaId'] + '/escrever-critica/'
    starIndex = int(postInfo['stars']) * 2 - 1

    driver.get(adoroCinemaUrl);

    # login
    login = driver.find_elements_by_class_name('input_txt')
    login[0].send_keys(adorocinema_credentials.email)
    login[1].send_keys(adorocinema_credentials.pwd)
    btnLogin = driver.find_element_by_class_name('btn-primary')
    btnLogin.click()

    # fill
    reviewArea = driver.find_element_by_class_name('review-textarea')
    reviewArea.send_keys(postInfo['paragraph'].decode('utf8'))
    time.sleep(5)
    reviewUrl = driver.find_element_by_class_name('review-url-input')
    reviewUrl.send_keys(postUrl)
    time.sleep(5)
    reviewStars = driver.find_elements_by_class_name('rating-star')
    reviewStars[starIndex].click()

    # publish
    #time.sleep(10)
    #reviewSubmit = driver.find_element_by_class_name('review-submit')
    #reviewSubmit.send_keys(Keys.RETURN)

    # exit
    #time.sleep(10)
    ret = input('Type enter to continue')


def SearchAdoroCinema(postInfo):
    titleSearch = postInfo['title'].decode('utf8')
    titleSearch = titleSearch.replace(' ', '+')
    webbrowser.open_new_tab('http://www.adorocinema.com/busca/?q=' + titleSearch)


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
    with open(postInfo['file']) as f:
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

def PublishToSocialMedia(post, img):
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

        lastShortener = 'Google'
        shortenerOk = False
        while shortenerOk == False:
            time.sleep(3)
            try:
                shortener = GetShortener(lastShortener)
                postInfo['shortlink'] = baseUrl + postInfo['permalink']
                shortenerOk = True
            except Exception as e:
                print "Exception in shortener, waiting: ", str(e)
                lastShortener = 'Google' if lastShortener != 'Google' else 'Tinyurl'

        imgUrl = urllib2.urlopen(img)
        img = imgUrl.read()
        print '*** Publishing to Twitter'
        PublishToTwitter(postInfo, img)
        print '*** Publishing to Facebook'
        PublishToFacebook(postInfo, img)
        print '*** Done!'
        SearchAdoroCinema(postInfo)
        webbrowser.open_new_tab(link)
        webbrowser.open_new_tab('https://plus.google.com/b/116314610297829036822')
        webbrowser.open_new_tab('https://www.facebook.com/cinetenisverde/')
        webbrowser.open_new_tab('https://twitter.com/')
    except Exception as e:
        print '*** Something gone wrong!'
        raise

if len(sys.argv) < 3:
    print 'How to use: share.py post-slug http://image-link'
else:
    PublishToSocialMedia(sys.argv[1], sys.argv[2])


