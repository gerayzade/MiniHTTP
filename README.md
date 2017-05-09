# MiniHTTP
Simple web-server, which can handle HTTP requests and load a static web page (only GET request is implemented). The web-server is accessible on port 9000 by default (you can assign port no. from the command line optionally)
## How to use
- <b>index.php</b> and <b>/res</b> folder should be included into the folder with C program (<i>minihttp.c</i>);
- compile C program and run it (assigning a port number :
<pre>
gcc minihttp.c -o minihttp
./minihttp [PORT NO.]
</pre>
- the default request is considered as http://127.0.0.1:9000/index.php
<img width="500px" src="mockup.jpg" />
<br/><br/>
View on <a href="http://www.gerayzade.me/dev/minihttp/" target="_blank">gerayzade.me</a>
