LazyIterator
============

A repo that at some point might become a library.
Inspired heavily by Eric Nieblers Blog Post [Range Concepts: Delimited Ranges](http://ericniebler.com/2014/02/16/delimited-ranges/)

Currently this project requires a C++1y compiler, or rather a C++11 compiler which provides automatic return type deduction for regular functions.
This may be relaxed in a future release, but I want to finalize the implementation of a couple of functions before doing that.

So what does it do?
-------------------

In short, you can do this:

```
std::string s = ...;

auto pos = ph::find(s.begin(), ph::UntilValue({'a'}) || s.end(), 'b');
```

Instead of say:

```
std::string s = ...;

auto posOfA = std::find(s.begin(), s.end(), 'a');

auto pos = std::find(s.begin(), posOfA, 'b');
```

In both cases, the value of pos is the same, but note that in the second example, we had to iterate over the string twice, whereas in the former implementation we only iterate over the string once, and break if either 'a' _or_ 'b' is found, or we have reached the end of the string.

This can be useful if either the range your trying to iterate over has no .end(), or is expensive to compute (such as with C strings).
