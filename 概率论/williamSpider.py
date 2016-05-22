import urllib2
import re

_500url = 'http://odds.sports.sina.com.cn/liveodds/'
_oddurl = 'http://odds.jcbao.com/'
_310url = 'http://www.310win.com/buy/jingcai.aspx?typeID=105&oddstype=2'
pattern = re.compile(r"[1]\.[2-9][0-9]|[2-9]\.[1-9][0-9]|[0-9][1-9]\.[1-9][0-9]|[2-9]\.[0-9][0-9]|[0-9][2-9]\.[0-9][0-9]")
urlfile = urllib2.urlopen(_500url)
html = urlfile.read()
urlfile.close()
wordlist = re.findall(pattern, html)
print wordlist

fp = open("/Users/houqingfeng/Desktop/williamOdds.txt", "w+")
wordLocate = 0
for i in range(0, len(wordlist), 12):
    for j in range(0, 3, 1):
        if wordLocate < len(wordlist):
            if wordLocate == len(wordlist) - 1:
                fp.write(wordlist.pop(wordLocate))
            else:
                fp.write(wordlist.pop(wordLocate) + " ")
            ++wordLocate
        else:
            break
    wordLocate = wordLocate + 9
    fp.write('\n')
fp.close()
