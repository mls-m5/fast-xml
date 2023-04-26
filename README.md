# fast-xml

# Plan
- [x] Build slow parser (generic syntax)
   - [x] Tokenizer
   - [x] Parser
- [x] Build fast parser
   - [x] Memory hacks
- [x] Compare
   - [x] Rapidxml
   - [x] TinyXml2
   - [x] Old slower algorithm
- [x] Refactor for usage in other projects


Things that _might_ be implemented in the future
- Xml version code handling


# Results of testing

Test application is a simple program that reads a xml-file, formats it and
prints it agains.

Note that this library is not well tested yet so it probably does contain a
lot of bugs.

Also note that I might not read files in the most optimal way, but it does
not seem to be the majority of the time spent according to callgrind.

According to this simple test, this library is in the same range as the other
libraries. More testing is required to draw any definite conclusions.

Added comparison with algorithm using heap allocations. The difference in speed
seems to be only like 2x. However, file input and output is a big part of the
time used (like half of the time for the other algorithms), 
so the algorithm might be more like 4x slower.

```bash
# RapidXml
$ time  ../rxmlfmt -i generated2.json  -o  out.json

real	0m0,896s
user	0m0,730s
sys	0m0,147s


# TinyXml2
$ time  ../txmlfmt -i generated2.json  -o  out.json

real	0m0,418s
user	0m0,265s
sys	0m0,152s


# FastXml (This library)
$ time  ../fastxmlfmt -i generated2.json  -o  out.json

real	0m0,454s
user	0m0,276s
sys	0m0,158s

# HeapXml (Objects that is used for dynamically build xml documents)
time  ../slowxmlfmt -i generated2.json  -o  out.json

real	0m0,805s
user	0m0,616s
sys	0m0,188s

```
