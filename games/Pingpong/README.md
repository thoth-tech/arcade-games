# Ping Pong

## Technical overview

- **Language:** C++
- **Current scope:** Small
- **Code quality:** Fair
- **Gameplay implementation:** Near-complete

## Gameplay overview

A vaguely soccer-themed pong clone requiring two players. Players control paddles freely within their side of the board to prevent the ball from crossing into their goal.

### Player 1 Controls

| Action            | Key               |
|-------------------|-------------------|
| Move paddle up    | W Key             |
| Move paddle left  | S Key             |
| Move paddle down  | S Key             |
| Move paddle right | D Key             |
| RESTART           | R Key             |

### Player 2 Controls

| Action            | Key               |
|-------------------|-------------------|
| Move paddle up    | Up Arrow          |
| Move paddle left  | Left Arrow        |
| Move paddle down  | Down Arrow        |
| Move paddle right | Right Arrow       |
| Exit The Game     | ESC Key           |

## Other notes

- Documentation from the original author is available in `/docs/`.
- BUG: it's possible to move the ball into paddle (or vice versa), causing issues with collision handling.

## Screenshots

![](/docs/screenshot-01.png)

## Credits

Originally created by Pratham Gupta in 2023-T2.
