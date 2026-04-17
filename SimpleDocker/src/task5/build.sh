docker build . -t titandea:1.0 
docker run -v "$PWD/nginx/nginx.conf":"/etc/nginx/nginx.conf" -d -p 80:81 titandea:1.0