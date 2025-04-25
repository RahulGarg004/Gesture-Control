# Whispr Web App

This is a privacy‑focused whispering board styled for the Tor network, built with HTML, CSS, and JavaScript. The entire front end can be served by a Node.js ("Nods.js") runtime as static files—no backend data logging occurs.

---

## package.json

Define a simple Node.js project to serve static files (e.g. using `http-server` or Express).

```json
{
  "name": "whispr",
  "version": "1.0.0",
  "description": "Anonymous whispering board for Tor network",
  "main": "server.js",
  "scripts": {
    "start": "node server.js"
  },
  "dependencies": {
    "express": "^4.18.2"
  }
}
```

---

## server.js

A minimal Express server to serve our static front end.

```js
const express = require('express');
const path = require('path');
const app = express();

app.use(express.static(path.join(__dirname, 'public')));

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => console.log(`Whispr running on port ${PORT}`));
```

---

## Directory structure

```
whispr/
├── package.json
├── server.js
└── public/
    ├── index.html
    ├── style.css
    └── script.js
```

---

## public/index.html

```html
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>Whispr</title>
  <link rel="stylesheet" href="style.css" />
</head>
<body>
  <!-- Login View -->
  <div id="login-view" class="view">
    <div class="login-container">
      <h1>Whispr</h1>
      <p id="assigned-username"></p>
      <form id="login-form">
        <label for="password">Choose a Password:</label>
        <input type="password" id="password" required />
        <button type="submit">Enter Board</button>
      </form>
      <div id="login-error" class="error"></div>
    </div>
  </div>

  <!-- Board View -->
  <div id="board-view" class="view hidden">
    <header>
      <h2>Board</h2>
      <button id="new-whispr-btn">🔊 Whisper to the world</button>
    </header>
    <div id="post-form-container" class="hidden">
      <textarea id="whispr-text" placeholder="Your whisper..."></textarea>
      <input type="file" id="attachment" />
      <button id="post-whispr">Post</button>
    </div>
    <div id="posts"></div>
    <nav class="bottom-nav">
      <button id="nav-boards" class="active">Boards</button>
      <button id="nav-nodes">Nodes 🔒</button>
    </nav>
  </div>

  <!-- Nodes View -->
  <div id="nodes-view" class="view hidden">
    <header><h2>Nodes</h2></header>
    <div class="nodes-list">
      <!-- Example nodes -->
      <div class="node">General</div>
      <div class="node">News</div>
      <div class="node premium">Tech (Premium)</div>
      <div class="node premium">Secrets (Premium)</div>
    </div>
    <nav class="bottom-nav">
      <button id="nav-boards-2">Boards</button>
      <button id="nav-nodes-2" class="active">Nodes 🔒</button>
    </nav>
  </div>

  <script src="script.js"></script>
</body>
</html>
```

---

## public/style.css

```css
/* Purple → Black gradient background */
body {
  margin: 0;
  font-family: sans-serif;
  background: linear-gradient(135deg, #6a0dad, #000);
  color: #eee;
  height: 100vh;
  display: flex;
  justify-content: center;
  align-items: center;
}

.view {
  position: absolute;
  top: 0; left: 0;
  width: 100%; height: 100%;
}
.hidden { display: none; }

.login-container {
  background: rgba(0,0,0,0.7);
  padding: 2rem;
  border-radius: 8px;
  text-align: center;
  width: 90%; max-width: 400px;
}
.login-container input,
.login-container button {
  width: 100%; margin-top: 1rem;
  padding: 0.75rem;
  border: none;
  border-radius: 4px;
}
.login-container button {
  background: #6a0dad;
  color: #fff;
  cursor: pointer;
}
.error { color: #f66; margin-top: 0.5rem; }

#board-view, #nodes-view {
  display: flex;
  flex-direction: column;
}
header {
  background: rgba(0,0,0,0.6);
  padding: 1rem;
  display: flex;
  justify-content: space-between;
  align-items: center;
}
#new-whispr-btn {
  background: #6a0dad;
  border: none;
  padding: 0.5rem 1rem;
  border-radius: 4px;
  color: #fff;
  cursor: pointer;
}
#post-form-container {
  background: rgba(0,0,0,0.5);
  padding: 1rem;
}
#whispr-text {
  width: 100%; height: 4rem;
  margin-bottom: 0.5rem;
}
#posts {
  flex: 1;
  overflow-y: auto;
  padding: 1rem;
}
.post {
  background: rgba(255,255,255,0.1);
  padding: 1rem;
  margin-bottom: 1rem;
  border-radius: 6px;
}
.post .user { font-weight: bold; }
.post img { max-width: 100%; margin-top: 0.5rem; border-radius: 4px; }

.bottom-nav {
  display: flex;
  background: rgba(0,0,0,0.6);
}
.bottom-nav button {
  flex: 1;
  padding: 1rem;
  background: none;
  border: none;
  color: #ccc;
  font-size: 1rem;
  cursor: pointer;
}
.bottom-nav button.active {
  color: #fff;
  border-top: 2px solid #6a0dad;
}

.nodes-list {
  padding: 1rem;
  overflow-y: auto;
  flex: 1;
}
.node {
  background: rgba(255,255,255,0.1);
  height: 4rem;
  margin-bottom: 1rem;
  border-radius: 6px;
  position: relative;
  padding: 0.5rem;
  display: flex;
  align-items: flex-end;
  font-size: 1.1rem;
}
.node.premium::after {
  content: '🔒';
  position: absolute;
  top: 0.5rem; right: 0.5rem;
}
```

---

## public/script.js

```js
// Util for random username
function genUsername() {
  return 'user_' + Math.random().toString(36).substr(2, 8);
}

// Elements
document.addEventListener('DOMContentLoaded', () => {
  const loginView = document.getElementById('login-view');
  const boardView = document.getElementById('board-view');
  const nodesView = document.getElementById('nodes-view');
  const assignedEl = document.getElementById('assigned-username');
  const form = document.getElementById('login-form');
  const pwInput = document.getElementById('password');
  const errorEl = document.getElementById('login-error');

  // Check credentials in localStorage
  let creds = JSON.parse(localStorage.getItem('whisprCreds'));
  if (!creds) {
    // assign new
    creds = { user: genUsername(), pass: null };
    localStorage.setItem('whisprCreds', JSON.stringify(creds));
  }
  assignedEl.textContent = `Username: ${creds.user}`;

  form.addEventListener('submit', e => {
    e.preventDefault();
    const pw = pwInput.value;
    if (!creds.pass) {
      // first time, set password
      creds.pass = pw;
      localStorage.setItem('whisprCreds', JSON.stringify(creds));
      showBoard();
    } else if (pw === creds.pass) {
      showBoard();
    } else {
      errorEl.textContent = 'Incorrect password';
    }
  });

  // Navigation buttons\ n  document.getElementById('nav-boards').onclick = () => switchView('board');
  document.getElementById('nav-nodes').onclick = () => switchView('nodes');
  document.getElementById('nav-boards-2').onclick = () => switchView('board');
  document.getElementById('nav-nodes-2').onclick = () => switchView('nodes');

  function showBoard() {
    loginView.classList.add('hidden');
    boardView.classList.remove('hidden');
    loadPosts();
  }

  function switchView(view) {
    boardView.classList.add('hidden');
    nodesView.classList.add('hidden');
    if (view === 'board') boardView.classList.remove('hidden');
    else if (view === 'nodes') nodesView.classList.remove('hidden');
  }

  // Posting
  const newBtn = document.getElementById('new-whispr-btn');
  const formContainer = document.getElementById('post-form-container');
  const postBtn = document.getElementById('post-whispr');
  const textInput = document.getElementById('whispr-text');
  const attachInput = document.getElementById('attachment');

  newBtn.onclick = () => formContainer.classList.toggle('hidden');
  postBtn.onclick = () => {
    const text = textInput.value.trim();
    if (!text) return;
    const post = { user: creds.user, text, ts: Date.now() };
    // handle attachment
    const file = attachInput.files[0];
    if (file) {
      const reader = new FileReader();
      reader.onload = () => {
        post.img = reader.result;
        savePost(post);
      };
      reader.readAsDataURL(file);
    } else {
      savePost(post);
    }
    textInput.value = '';
    attachInput.value = '';
    formContainer.classList.add('hidden');
  };

  function savePost(p) {
    const arr = JSON.parse(localStorage.getItem('whisprPosts') || '[]');
    arr.unshift(p);
    localStorage.setItem('whisprPosts', JSON.stringify(arr));
    renderPost(p, true);
  }

  function loadPosts() {
    const arr = JSON.parse(localStorage.getItem('whisprPosts') || '[]');
    const container = document.getElementById('posts');
    container.innerHTML = '';
    arr.forEach(p => renderPost(p));
  }

  function renderPost(p, prepend = false) {
    const div = document.createElement('div');
    div.className = 'post';
    div.innerHTML = `<div class="user">${p.user}</div><div class="text">${p.text}</div>`;
    if (p.img) {
      const img = document.createElement('img'); img.src = p.img;
      div.appendChild(img);
    }
    const posts = document.getElementById('posts');
    if (prepend) posts.prepend(div);
    else posts.appendChild(div);
  }
});
```

---

**Instructions:**
1. Install dependencies with `npm install`.
2. Run `npm start` to launch on port 3000.
3. Access via Tor browser at `http://localhost:3000`.

The app assigns you a random username, lets you set a password, and stores all posts locally—no server logging. Navigate between Boards and Nodes (premium nodes locked) via the bottom navbar. Enjoy whispering securely! 

