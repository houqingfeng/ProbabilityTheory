import sys
import urllib2
import re

x = input("x: ")
_500url = 'http://odds.500.com/ouzhi.php?cid=293'
_310url = 'http://www.310win.com/buy/jingcai.aspx?typeID=105&oddstype=2'
pattern = re.compile(r"[0-9]\.[0-9][0-9]|[0-9][0-9]\.[0-9][0-9]")
urlfile = urllib2.urlopen(_310url)
html = urlfile.read()
urlfile.close()
wordlist = re.findall(pattern, html)
print wordlist

fp = open("/Users/houqingfeng/Desktop/odds.txt", "w+")
wordLocate = 4
for i in range(0, x, 1):
    for j in range(0, 3, 1):
        if wordLocate < len(wordlist):
            if wordLocate == len(wordlist) - 1:
                fp.write(wordlist.pop(wordLocate))
            else:
                fp.write(wordlist.pop(wordLocate) + " ")
            ++wordLocate
        else:
            break
    wordLocate = wordLocate + 3
    fp.write('\n')
fp.close()
