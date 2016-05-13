
__author__ = 'zdz8207'
from bs4 import BeautifulSoup

import sys
import io
import urllib.request
import urllib.parse
import re
import urllib.request, urllib.parse, http.cookiejar

def getHtml(url):
	cj = http.cookiejar.CookieJar()
	opener = urllib.request.build_opener(urllib.request.HTTPCookieProcessor(cj))
	opener.addheaders = [('User-Agent','Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2272.101 Safari/537.36'),('Cookie', '4564564564564564565646540')]

	urllib.request.install_opener(opener)

	html_bytes = urllib.request.urlopen(url).read()
	html_string = html_bytes.decode("utf-8",errors="ignore")
	return html_string


html_doc = getHtml("http://blog.csdn.net/dog250/article/details/46947959")
soup = BeautifulSoup(html_doc, 'html.parser')
#soup = BeautifulSoup(open("http://news.chinaunix.net"))
title_str=str(soup.title)
#sys.stdout = io.TextIOWrapper(sys.stdout.buffer,encoding='gb18030')  
print(title_str)
#table = soup.find_all('table', class_='fzTab')
#print(table)#<tr onmouseout="this.style.background=''" 这种tr丢失了
#soup.strip() 加了strip后经常出现find_all('tr') 只返回第一个tr
#for link in soup.findAll('a',href=re.compile('http://\\S+/\\S+.shtml')):#使用正则表达式
for link in soup.findAll('a',href=re.compile('http://\\S+/article/\\S+')):#使用正则表达式  
        print(link['href'],link.contents[0])

content= soup.find('div',id="article_content")

for sentence in content.contents:
        sentence2=str(sentence)
        sentence2=sentence2.strip('\<br\/\>')
        print(sentence2.strip('\<br\>'))


"""
tds = tr.find_all('td') 
opennum = tds[0].get_text() 
#print(opennum) 

reds = [] 
for i in range(2,8): 
	reds.append(tds[i].get_text()) 
	#print(reds) 
	blue = tds[8].get_text() 
	#print(blue) 
	#把list转换为字符串:(',').join(list) 
	#最终输出结果格式如：2015075期开奖号码：6,11,13,19,21,32, 蓝球：4 
	print(opennum+'期开奖号码：'+ (',').join(reds)+", 蓝球："+blue)

"""
