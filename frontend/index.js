const express = require('express');

const app = express();

app.use(express.static('static'));
app.get('/', (req, res) => {
  res.send('Hello, world!');
});

app.listen(3000, () => {
  console.log('Server listening on port 3000');
});