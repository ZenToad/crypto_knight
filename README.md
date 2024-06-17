# Readme

## Thoughts

1. Use Modules or Packages
    Instead of putting everything in a single file, break your code into smaller, manageable modules or packages. 
    This way, you can keep related functionality together and maintain a cleaner structure.

Example:

```python
# File: main.py
import mechanics.jump as jump
import mechanics.run as run

def main():
    jump.test_jump()
    run.test_run()

if __name__ == "__main__":
    main()

# File: mechanics/jump.py
def test_jump():
    print("Testing jump mechanic")

# File: mechanics/run.py
def test_run():
    print("Testing run mechanic")
```

2. Use a Temporary Prototyping Folder
Create a specific folder for your prototypes. This way, you can keep your main project clean and organized while having a dedicated space for experimenting.

Example:

```
project_root/
│
├── prototypes/
│   ├── prototype1/
│   │   ├── main.py
│   │   ├── mechanic1.py
│   │   └── mechanic2.py
│   └── prototype2/
│       ├── main.py
│       └── mechanicA.py
├── main_project/
│   ├── main.py
│   └── game_mechanics.py
└── README.md
```

3. Version Control with Git Branches
Use Git branches to manage different prototypes. Create a new branch for each prototype so you can experiment freely without affecting your main codebase.

```
# Create a new branch for a prototype
git checkout -b prototype-jumping-mechanic

# Work on your prototype...

# Switch back to the main branch
git checkout main
```

4. Document Your Ideas
Keep a document (Markdown, text file, or a note-taking app) where you jot down ideas, mechanics, and other thoughts. This helps in keeping track of what you've tried and what worked or didn't work.

EXAMPLE: 

    Prototyping Ideas

    Jumping Mechanic
    - Test with different gravity settings
    - Add double jump

    Running Mechanic
    - Test different acceleration rates
    - Implement stamina system

5. Code Snippets
Keep a collection of code snippets or utility functions that you find useful for prototyping. This can save you time and keep your prototypes cleaner.

```python
# File: utils.py

def log(message):
    print(f"[LOG]: {message}")

def load_image(path):
    # Dummy function to simulate image loading
    return f"Image at {path} loaded"

# Use these in your prototypes
from utils import log, load_image

log("Starting prototype")
image = load_image("path/to/image.png")
```

By using these strategies, you can maintain a balance between rapid prototyping and keeping your code organized. This way, you won't lose track of your ideas and can iterate on your game mechanics more effectively.
