from datetime import datetime,date,time
from random import randrange
import json
import os
#import file

TD= datetime.now()

usernames=['guanliyuan',
           'yangguo',
           'xiaofeng',
           'duanyu',
           'huanglei']

mediafiles=['wuzetian.mp4',
            'bianxingjingang.mp4',
            'langyabang.mp4',
            'weizhuangzhe.mp4',
            'sanguo.mp4'
            ]
uris=[
    '/dianying/2013',
    '/dianying/2014',
    '/dianying/2015',
    '/dianying/2012',
    '/dianying/2011'
    ]

f=open('test.json','w+')

for i in range(1,100):
    year=2015
    month=randrange(1,12)
    day=randrange(1,29)
    hour=randrange(0,24)
    minute=randrange(0,59)
    second=randrange(0,59)
    TD=datetime(year,month,day,hour,minute,second)
    timestamp=TD.__format__("%Y-%m-%dT%H:%M:%S.000Z")
    #print timestamp

    randrange(0,3)
    media_idx=randrange(0,len(mediafiles))
    mediafile=mediafiles[media_idx]
    uri=uris[media_idx]+"/"+mediafiles[media_idx]
    #print uri
    #print mediafile

    sourceip="{0}.{1}.{2}.{3}".format(randrange(0,255),randrange(0,255),randrange(0,255),randrange(0,255))
    #print sourceip

    username=usernames[randrange(0,len(usernames))]
    #print username

    json_item= json.dumps({'@timestamp':timestamp,
                'username':username,
                'uri':uri,
                'mediafile':mediafile,
                'souceip':sourceip
                })


    f.write(json_item+"\n")

f.close()