printf '\nTest GET http://localhost:8080/ (200)\n'
curl -X GET -H 'Host: localhost:8080' \
    -H 'User-Agent: Go-http-client/1.1' \
    -H 'Accept-Encoding: gzip' \
    http://localhost:8080/

printf '\nTest POST http://localhost:8080/ with a size of 0 (405)\n'
curl -X POST -H 'Host: localhost:8080' \
    -H 'User-Agent: Go-http-client/1.1' \
    -H 'Transfer-Encoding: chunked' \
    -H 'Content-Type: test/file' \
    -H 'Accept-Encoding: gzip' \
    -d '' \
    http://localhost:8080/

printf '\nTest HEAD http://localhost:8080/ (405)\n'
curl -X HEAD -H 'Host: localhost:8080' \
    -H 'User-Agent: Go-http-client/1.1' \
    http://localhost:8080/

printf '\nTest GET http://localhost:8080/directory (200)\n'
curl -X GET -H 'Host: localhost:8080' \
    -H 'User-Agent: Go-http-client/1.1' \
    -H 'Accept-Encoding: gzip' \
    http://localhost:8080/directory

printf '\nTest GET http://localhost:8080/directory/youpi.bad_extension\n'
curl -X GET -H 'Host: localhost:8080' \
    -H 'User-Agent: Go-http-client/1.1' \
    -H 'Accept-Encoding: gzip' \
    http://localhost:8080/directory/youpi.bad_extension

printf '\nTest GET http://localhost:8080/directory/youpi.bla\n'
curl -X GET -H 'Host: localhost:8080' \
    -H 'User-Agent: Go-http-client/1.1' \
    -H 'Accept-Encoding: gzip' \
    http://localhost:8080/directory/youpi.bla

printf '\nTest GET Expected 404 on http://localhost:8080/directory/oulalala\n'
curl -X GET -H 'Host: localhost:8080' \
    -H 'User-Agent: Go-http-client/1.1' \
    -H 'Accept-Encoding: gzip' \
    http://localhost:8080/directory/oulalala

printf '\nTest GET http://localhost:8080/directory/nop\n'
curl -X GET -H 'Host: localhost:8080' \
    -H 'User-Agent: Go-http-client/1.1' \
    -H 'Accept-Encoding: gzip' \
    http://localhost:8080/directory/nop

printf '\nTest GET http://localhost:8080/directory/nop/\n'
curl -X GET -H 'Host: localhost:8080' \
    -H 'User-Agent: Go-http-client/1.1' \
    -H 'Accept-Encoding: gzip' \
    http://localhost:8080/directory/nop/

printf '\nTest GET http://localhost:8080/directory/nop/other.pouic\n'
curl -X GET -H 'Host: localhost:8080' \
    -H 'User-Agent: Go-http-client/1.1' \
    -H 'Accept-Encoding: gzip' \
    http://localhost:8080/directory/nop/other.pouic

printf '\nTest GET Expected 404 on http://localhost:8080/directory/nop/other.pouac\n'
curl -X GET -H 'Host: localhost:8080' \
    -H 'User-Agent: Go-http-client/1.1' \
    -H 'Accept-Encoding: gzip' \
    http://localhost:8080/directory/nop/other.pouac

printf '\nTest GET Expected 404 on http://localhost:8080/directory/Yeah\n'
curl -X GET -H 'Host: localhost:8080' \
    -H 'User-Agent: Go-http-client/1.1' \
    -H 'Accept-Encoding: gzip' \
    http://localhost:8080/directory/Yeah

printf '\nTest GET http://localhost:8080/directory/Yeah/not_happy.bad_extension\n'
curl -X GET -H 'Host: localhost:8080' \
    -H 'User-Agent: Go-http-client/1.1' \
    -H 'Accept-Encoding: gzip' \
    http://localhost:8080/directory/Yeah/not_happy.bad_extension

printf '\nTest Put http://localhost:8080/put_test/file_should_exist_after with a size of 1000 (201)\n'
curl -X PUT -H 'Host: localhost:8080' \
    -H 'User-Agent: Go-http-client/1.1' \
    -H 'Transfer-Encoding: chunked' \
    -H 'Accept-Encoding: gzip' \
    -d 'eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee' \
    http://localhost:8080/put_test/file_should_exist_after

printf '\nTest Put http://localhost:8080/put_test/file_should_exist_after with a size of 10000000 (201)\n'
curl -X PUT -H 'Host: localhost:8080' \
    -H 'User-Agent: Go-http-client/1.1' \
    -H 'Transfer-Encoding: chunked' \
    -H 'Accept-Encoding: gzip' \
    -d 'zzzzz...' \
    http://localhost:8080/put_test/file_should_exist_after

printf '\nTest POST http://localhost:8080/directory/youpi.bla with a size of 100000000 (200)\n'
curl -X POST -H 'Host: localhost:8080' \
    -H 'User-Agent: Go-http-client/1.1' \
    -H 'Transfer-Encoding: chunked' \
    -H 'Content-Type: test/file' \
    -H 'Accept-Encoding: gzip' \
    -d 'nnnnnnnnnnnnnnnnnn...' \
    http://localhost:8080/directory/youpi.bla

printf '\nTest POST http://localhost:8080/directory/youpla.bla with a size of 100000000 (200)\n'
curl -X POST -H 'Host: localhost:8080' \
    -H 'User-Agent: Go-http-client/1.1' \
    -H 'Transfer-Encoding: chunked' \
    -H 'Content-Type: test/file' \
    -H 'Accept-Encoding: gzip' \
    -d 'ccccccccccccccccccc...' \
    http://localhost:8080/directory/youpla.bla

printf '\nTest POST http://localhost:8080/directory/youpi.bla with a size of 100000 with special headers (200)\n'
curl -X POST -H 'Host: localhost:8080' \
    -H 'User-Agent: Go-http-client/1.1' \
    -H 'Transfer-Encoding: chunked' \
    -H 'Content-Type: test/file' \
    -H 'X-Secret-Header-For-Test: 1' \
    -H 'Accept-Encoding: gzip' \
    -d 'yyyyyyyyyyyyyyyyyyyyyy...' \
    http://localhost:8080/directory/youpi.bla

printf '\nTest POST http://localhost:8080/post_body with a size of 0 (200)\n'
curl -X POST -H 'Host: localhost:8080' \
    -H 'User-Agent: Go-http-client/1.1' \
    -H 'Transfer-Encoding: chunked' \
    -H 'Content-Type: test/file' \
    -H 'Accept-Encoding: gzip' \
    -d '' \
    http://localhost:8080/post_body

printf '\nTest POST http://localhost:8080/post_body with a size of 100 (200)\n'
curl -X POST -H 'Host: localhost:8080' \
    -H 'User-Agent: Go-http-client/1.1' \
    -H 'Transfer-Encoding: chunked' \
    -H 'Content-Type: test/file' \
    -H 'Accept-Encoding: gzip' \
    -d 'ttttttttttttttttttt...' \
    http://localhost:8080/post_body

printf '\nTest POST http://localhost:8080/post_body with a size of 200 (413)\n'
curl -X POST -H 'Host: localhost:8080' \
    -H 'User-Agent: Go-http-client/1.1' \
    -H 'Transfer-Encoding: chunked' \
    -H 'Content-Type: test/file' \
    -H 'Accept-Encoding: gzip' \
    -d 'cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc' \
    http://localhost:8080/post_body
