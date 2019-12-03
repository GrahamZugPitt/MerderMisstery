To whom it may concern,

  Yeah, I know all this new code looks like a lot. Really though,
it's the same code we've been using the whole time, just structured
so that multiple people can work on multiple things at once (or that's the goal).
All that you need to know is that the program is broken down as follows:

common: just all the imports we'll need, and some constants
        try to avoid adding anything, unless you know for sure you won't change it
        also don't declare anything in here. Only constant definitions, otherwise
        it'll complain angrily.

main: as per usual, our main function

main_helper: the init() function and load_data(). Technical stuff that seems
  to be functional, so we shouldn't mess around with it

menuloop: the loop that allows you to either start the game OR start chat

chat: Just the loop that currently shows the "chat soon to arrive" page.
  Works both from the menu and from the game.

gameloop: the meat of the code. Runs everything we do, and probably should be
  even more subdivided than it is currently. I'll work on that.

Player: Our player class, slightly different from what we've been working with.
  priority number 1 is getting the castle collidor out of the Player class and
  into its own class (NPC file or something)

WorldObjects: Something I'm guessing will be obseleted or filled in as the NPC
  file. Not much in here as yet.

seed: allows the user to enter a generation seed to replay a game

Lmao this got so out of hand
