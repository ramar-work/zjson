# zjson

Simplistic JSON converter.


## Summary

zjson aims to be an incredibly simple JSON encoder and decoder.  
(It's main use is as a JSON parsing engine for my web server software: <a href="https://github.com/zaiah-dj/hypno">Hypno</a>)

Because of this, there is only one data structure (a hash table) to stream out of and into.  Please see this library (<a href="https://github.com/zaiah-dj/ztable">ztable</a>) for additional info on it.


## Building 

<pre>
$ git clone https://github.com/zaiah-dj/zjson.git
$ cd zjson && make && make test
</pre>


## Usage

Decoding JSON strings will output a hash table.  
In C, this will look something like:

<pre>
char err[1024] = {0};
char jsonenc[] = "{ data: [ { phantom: \"power\", ibm: \"thinkpad\" } ] }";
zTable \* zjt = zjson_decode( jsonenc, sizeof(jsonenc), err, sizeof( err ) );
</pre>


We can also encode by supplying a zTable.

<pre>
char * jsondec = zjson_encode( zjt, err, sizeof( err ) );
</pre>






