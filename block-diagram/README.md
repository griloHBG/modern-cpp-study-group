# Block Diagram Builder

This project is an attempt of creating a Block Diagram Builder to learn more about "Modern" C++ (revisions 11, 14, 17, 20... - I'm still in debt with 20's revision)

Here we have 2 main classes: `BlockBase` and `SignalBase`

A Block is fundamentally made of input **Signals** (0 or more), output **Signals** (0 or more) and *one* function for each output.

The Block hierarchy:

![](./Images/Blocks.png)

A Signal is what brings the Block diagram to life. When a `OutputSignal` is evaluated, it triggers recurrently (by design) the evaluation of every signal directly and indirectly connected to it.

The Signal hierarchy:

![](./Images/Signals.png)



