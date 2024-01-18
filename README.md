# introduction

The Internet Relay Chat (IRC) protocol has been designed over a number of years, with multitudes of implementations and use cases appearing. This document describes the IRC Client-Server protocol.

IRC is a text-based chat protocol which has proven itself valuable and useful. It is well-suited to running on many machines in a distributed fashion. A typical setup involves multiple servers connected in a distributed network. Messages are delivered through this network and state is maintained across it for the connected clients and active channels.

The key words “MUST”, “MUST NOT”, “REQUIRED”, “SHALL”, “SHALL NOT”, “SHOULD”, “SHOULD NOT”, “RECOMMENDED”, “MAY”, and “OPTIONAL” in this document are to be interpreted as described in RFC2119.

# IRC concepts

This section describes concepts behind the implementation and organisation of the IRC protocol, which are useful in understanding how it works.

# recourcse

<a href="https://beej.us/guide/bgnet/html/split-wide/index.html">
  socket programming guide.
</a>
