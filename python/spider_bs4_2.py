import urllib.request  
from bs4 import BeautifulSoup  
import re  
  
def getgxnu():  
    url="http://www.gxnu.edu.cn/default.html"  
    data=urllib.request.urlopen(url).read()  
    page_data=data.decode('GBK')  
    '''''print(page_data)'''  
    soup=BeautifulSoup(page_data)  
    #for link in soup.findAll('a',target='_self'):#get all links of gxnu index  
    #    print(link)  
    for link in soup.findAll('a',href=re.compile('http://\\S+/type/\\d+.html')):#使用正则表达式  
        print(link['href'],link.contents)
        print(link)
          
      
#函数调用  
getgxnu()
