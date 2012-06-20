Kangaroo Iceberg
================

So, Kangaroo Iceberg...

Kangaroo Iceberg was a language I started designing and implementing
in the mid-aughts -- the first distfile for it has the revision
2007.0406, but I suspect I was working on it a year or so before that.

I worked on it a while, borrowing C code from other implementations
of my languages for it, then for some reason promptly lost interest.
Unfortunately, while I left a grammar and a partial implementation
(mostly consisting of the parser) behind, I didn't leave any notes
on the semantics of the language.  I have a recollection that it was
supposed to be like a pared-down version of Tamerlane, but I don't
remember any specifics.

So, this is my Kangaroo Iceberg reconstruction project.  I'm going
to explain the existing syntax, modify it a bit, and add some
semantics that I think are reasonable to it.

Syntax
------

Here's the grammar of Kangaroo Iceberg as I left it.

    KangarooIceberg     ::= {TopLevelNode}.
    TopLevelNode        ::= NodeName NodeLit.
    NodeLit             ::= "{" {ArcDefn} ["/" RuleSets] "}".

    ArcDefn             ::= NodeRef ":" WeightDefn.
    NodeRef             ::= NodeLit | "^" NodeName | NodeName NodeLit.
    WeightDefn          ::= <<literal integer>>.

    RuleSets            ::= RuleSet {";" RuleSet}.
    RuleSet             ::= Rule {"," Rule}.
    Rule                ::= {ArcApp} "->" {ArcApp}.

    ArcApp              ::= NodeApp ":" WeightApp.
    NodeApp             ::= NodeRef | Var<Node>.
    WieghtApp           ::= WeightDefn | Var<Weight>.

    Var                 ::= "$" Name.

This all seems somewhat reasonable, although I will say a few things.

The rules are definitely patterns that will be matched against arc
weights, and I'm not sure why their production names contain "App".

I'm not entirely certain why there is a list of distinct rulesets for
each node, instead of say just a list of rules, but I'll figure that
out when I get to semantics.

I think I'd like Kangaroo Iceberg to be, in contrast with Tamerlane,
a fairly simple language; thus I see no need for there to be variables
(rewriting languages can get by well without them; see, for example,
Thue.)  So I'll remove that.

The new grammar thus looks like:

    KangarooIceberg     ::= {NamedNode}.
    NamedNode           ::= NodeName NodeLit.
    NodeLit             ::= "{" {ArcDefn} ["/" RuleSets] "}".

    ArcDefn             ::= NodeRef ":" WeightDefn.
    NodeRef             ::= NodeLit | "^" NodeName | NamedNode.
    NodeName            ::= <<letter (letter|digit)*>>
    WeightDefn          ::= <<digit digit*>>.

    RuleSets            ::= RuleSet {";" RuleSet}.
    RuleSet             ::= Rule {"," Rule}.
    Rule                ::= {ArcDefn} "->" {ArcDefn}.

Now let's explain it in English.

A Kangaroo Iceberg program consists of a list of node definitions.
Each of these names a node, and describes the contents of the node.
The contents of each node has two parts: a description of the
(weighted) arcs extending from that node to other nodes, and the rules
that are used to rewrite the weights of those arcs when the node is
traversed during execution

The simplest possible Kangaroo Iceberg program (barring the null
program) is just a single node with no arcs and no rules.  Still, it
needs to have a name.

    | A {}
    = A {}

Other simple programs include a single node which simply has an arc
to itself...

    | A { ^A:1 }
    = A { ^A:1 }

...and a program with two nodes with each have arcs to the other.

    | A { ^B:1 }
    | B { ^A:1 }
    = A { ^B:1 }
    = B { ^A:1 }

Mostly as syntactic sugar, nodes may be defined inside the definition
of other nodes.  The following is equivalent to the above.

    | A { B { ^A:1 }:1 }
    = A { ^B:1 }
    = B { ^A:1 }

Kangaroo Iceberg does support anonymous (unnamed) nodes, but only in
the embedded-node-syntax shown above.  An equivalent to the above
program in which the `B` node is, in fact, unnamed, follows.  Note
that the implementation gives each anonymous node some kind of mangled
internal name.

    | A { { ^A:1 }:1 }
    = A { ^$1:1 }
    = $1 { ^A:1 }

Nodes can have multiple arcs to the same node.  (Maybe?)

    | A { ^A:1 ^A:1 }
    = A { ^A:1 ^A:1 }

Examples
--------

Here are some example Kangaroo Iceberg programs -- the ones I left
behind.  Unfortunately, I don't know what the meaning of these
programs was supposed to be; in fact, I don't know that they weren't
just examples of correct syntax, with no sensible meaning.

    | A { ^A:0 ^B:1 }
    | B { ^C:1 { ^C:1 }:9 }
    | C { ^A:1 D { ^B:1 ^B:1 ^B:1 ^B:1 ^B:1 }:3 }
    | E { ^E:0 }
    = ok

    | A { ^A:0 / ^A:0 -> ^A:1 }
    | B { / ^B:0 -> ^B:1, ^B:1 -> ^B:2 }
    | C { {}:0 / ^K:0 -> ^K:1, ^K:1 -> ^K:2; ^A:1 -> ^A:0 }
    = ok
