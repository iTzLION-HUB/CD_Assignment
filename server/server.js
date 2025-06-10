const express = require('express');
const app = express();
const PORT = 3000;

// Middleware to parse JSON
app.use(express.json());

// In-memory storage for users
let users = [
    { id: 1, name: "Alice", email: "alice@example.com" },
    { id: 2, name: "Bob", email: "bob@example.com" }
];

// GET /users - Get all users
app.get('/users', (req, res) => {
    res.json(users);
});

// POST /users - Add a new user
app.post('/users', (req, res) => {
    const { name, email } = req.body;
    
    if (!name || !email) {
        return res.status(400).json({ error: 'Name and email are required' });
    }
    
    // Find the lowest available ID
    let newId = 1;
    while (users.some(user => user.id === newId)) {
        newId++;
    }
    
    const newUser = {
        id: newId,
        name,
        email
    };
    
    // Insert user at the correct position to maintain ascending ID order
    let insertIndex = 0;
    while (insertIndex < users.length && users[insertIndex].id < newId) {
        insertIndex++;
    }
    users.splice(insertIndex, 0, newUser);
    
    res.status(201).json(newUser);
});

// PUT /users/:id - Update a user
app.put('/users/:id', (req, res) => {
    const userId = parseInt(req.params.id);
    const { name, email } = req.body;
    
    if (!name || !email) {
        return res.status(400).json({ error: 'Name and email are required' });
    }
    
    const userIndex = users.findIndex(user => user.id === userId);
    
    if (userIndex === -1) {
        return res.status(404).json({ error: 'User not found' });
    }
    
    users[userIndex] = {
        id: userId,
        name,
        email
    };
    
    res.json({ message: 'User updated successfully', user: users[userIndex] });
});

// DELETE /users/:id - Delete a user
app.delete('/users/:id', (req, res) => {
    const userId = parseInt(req.params.id);
    const userIndex = users.findIndex(user => user.id === userId);
    
    if (userIndex === -1) {
        return res.status(404).json({ error: 'User not found' });
    }
    
    const deletedUser = users.splice(userIndex, 1)[0];
    res.json({ message: 'User deleted successfully', user: deletedUser });
});

// Start server
app.listen(PORT, () => {
    console.log(`Server running on http://localhost:${PORT}`);
console.log(`Return to the C++ application.`);
});