# C-Express
### Prologue note of the developer
With the desire to deep-dive into HTTP and Servers, bored(scared to enter in the kernel) in my own operating system project, i begin the development of the C-Express. 
Made in pure C++, in hope my abilities and knowledge of the language improve.
Keep in mind, that is not probably the best way to implement it, but well... I conceived that idea one year ago(09/2023), so why not implement it now that i have the properly capacities.
Hope you enjoy your journey throught that silly and strange code

### Concept
This project is a lightweight web-server library made in C++. 
Inspired by ExpressJS, the propose of the lib is to implement a generic and scalable web-server, that will be possible to create endpoints and its handlers.

### In-Scope
- Work to Windows and Linux
- Create endpoints(include endpoints with URL params)
- Response requests with plain/text and application/json
- HTTP 1.0 and HTTP 1.1
- ~Work firstly with HTTP/1.1, and 1.0 in the final version(yup, i am a fail to HTTP philosophy)~
- No cache will be implemented in the beta and alpha version

### Implementation
Made in pure C++
- Sockets(Windows and Linux)
- Threads

### Requirements
Requirements to use are simple, just have a Windows or Linux distro OS in your machine and put the lib on the correct path.
Requirements to build are just a C++ compiler
P.S: The project comes with a Makefile, so if you can run it, use "make build", and it will be ready in the build dir to move it to the right folder.

### How to use it
Put the lib compiled object in the right folder on your computer, and it will be ready to refer like any other lib.

## Documentation
### Getting started
Just like ExpressJS, you can define a route(endpoint) and listen to a port like:
![Getting started](docs/images/doc1.png)

## To do
### Pre-alpha
- [x] Sockets
  - [x] UnixSocket
- [x] Server
  - [x] Routes definition
  - [x] Direct request to handler using req UR
- [x] HTTPParser
  - [x] Parse the raw request, the first line, getting the protocol, method and URL
- [x] Request
  - [x] Port the request fields
- [x] Response
  - [x] Answer request with a pure text data

### Alpha
- [ ] Sockets
  - [ ] Create a socket based on developer OS 
  - [x] UnixSocket
  - [ ] WinSocket
- [ ] Server
  - [ ] Receive requests, and direct it to the HTTPController
  - [ ] Manage multi-clients with they threads
- [ ] HTTPController
  - [ ] Create resources and link it with endpoints
  - [ ] Parse raw requests, creating Request and Response, passing it to the URL handler(aka resource generator)
  - [ ] Parse URL params to a data structure
  - [ ] Parse query params to a data structure
  - [ ] Parse body(if present) to a data structure(only JSON, but dont forget to code it like a decent person, being able to future upgrades)
  - [ ] Port the endpoints of the application, with the respective methods they implement
  - [ ] HTTP 1.0
    - [ ] In the Request generator, verify if the protocol are the correct one, the same to the method and headers(only Content-type: application/json)
    - [ ] In the Response generator, verify the parts of the Request that transform the Response flow, like headers that allow a specify method
    - [ ] Send default errors using the headers send in the Request
- [ ] Request
  - [ ] Port the HTTP data related, like method, protocol, URL and HTTP headers 
  - [ ] Port data structure to URL params
  - [ ] Port data structure to query params
  - [ ] Port data structure to body
- [ ] Response
  - [ ] Port the HTTP data related, like status code, status desc, protocol and HTTP headers
  - [ ] Send the Response to the client(maybe using something of Server? Cause it ~should be~ is the only capable to send data through network)
