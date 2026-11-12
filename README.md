# 🎮 2048 in C

A command-line version of the classic **2048** game, written in pure C as my first-semester project at Jaypee Institute of Information Technology (BTech AI/ML).

Slide the tiles, merge the matching ones, and try to reach **2048** before the board fills up.

---

## 📌 About the Project

I built this to practise what we learned in class by making something that actually works instead of just solving textbook problems. The rule I set for myself was to **stay within syllabus topics only**, so there are no fancy libraries or advanced tricks.

---

## 🧰 Topics Used

| Topic | Where it shows up |
|---|---|
| SDLC and flowcharts | Planning the game before writing any code |
| Data types and operators | Score, board values, merge and move logic |
| Control flow (`if`, loops, `switch`) | Game loop, move handling, win/lose checks |
| Arrays | The game board `[SIZE x SIZE]` |
| Strings | Printing messages and the menu |
| Functions | Splitting the game into small, reusable pieces |
| Pointers | Passing the board and score into functions |

---

## 📁 Versions

| File | Description |
|---|---|
| `[2048_syllabus.c]` | Syllabus-only version. A formula-based "fake random" picks new tile spots, so no `rand()` is used. |
| `[2048_rand.c]` | Second version using the real `rand()` and `srand()` for tile spawning. |

---

## ⚙️ How to Compile and Run

```bash
gcc [2048_syllabus.c] -o 2048
./2048
```

For the `rand()` version:

```bash
gcc [2048_rand.c] -o 2048_rand
./2048_rand
```

---

## 🕹️ Controls

| Key | Action |
|---|---|
| `[W]` | Move up |
| `[A]` | Move left |
| `[S]` | Move down |
| `[D]` | Move right |
| `[Q]` | Quit |

---

## 🧠 How It Works

1. The game starts with a few tiles on the board.
2. You pick a direction and all tiles slide that way.
3. Matching neighbours merge into one tile and add to your score.
4. A new tile appears after every valid move.
5. You win at **2048** and lose when no moves are left.

---

## 🚧 Challenges I Faced

- [Add one or two real ones, like getting merges right without double-merging in one move]
- [Or how you handled the "no moves left" check]

---

## 🔮 Future Improvements

- [ ] Add a high score saved to a file
- [ ] Add an undo move
- [ ] Try a bigger board size

---

## 👤 Author

**Vihaan** | [@vihaanishere](https://github.com/vihaanishere)

*Project status: ✅ Completed (1st semester)*