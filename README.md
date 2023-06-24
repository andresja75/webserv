# Configuration

The configuration file format is something like a subset of yaml. An example:

```yml
server:
  root: path/YoupiBanane/
  name: serverOne
  cgi_path: cgi_tester
  max_request_size: 2500
  timeout: 30
  error_page:
    path: error.html
    status_code: 404
  listen:
    host: 127.0.0.1
    port: 8080
  listen:
    host: 0.0.0.0
    port: 7070
  location:
    route: /
    root: ./path/YoupiBanane
    index: index.html
    allow_method: GET
    allow_method: PUT
    directory_listing: true
    max_body_size: 100
    cgi: bla
  location:
    route: /direc/
    root: ./path/YoupiBanane/Yeah
    index: index.html
    index: not_happy.bad_extension
    allow_method: GET
    allow_method: DELETE
    allow_method: PUT
    cgi: bla

server:
  root: path/YoupiBanane/
  name: serverTwo
  listen:
    host: 127.0.0.1
    port: 8000
```

## Keywords
- server: it should be at least one
  - root: the default root directory of the server
  - name: the name of the server
  - cgi_path: the path to the executable
  - max_request_size: maximum size of the request that is allowed (better use max_body_size)
  - timeout: seconds of timeout of connection
  - error_page:
    - path: the path to the error page
    - status_code: the status code of the error
  - listen: it should be at least one
    - host: ip of the host to listen
    - port: port to listen
  - location: it is not mandatory. A server can have multiple locations
    - route: the route in the request to redirect
    - root: the path in the server's folders corresponding to the route
    - index: if the path requested is a directory, the file to response
    - allow_method: methods allows in the location
    - directory_listing: 'true' or 'on' to activate the listing of a directory requested
    - cgi: extension of files to execute the cgi
    - max_body_size: maximum size of body of request
