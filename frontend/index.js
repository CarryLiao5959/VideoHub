const express = require('express');

// socket of express
const expressWs = require('express-ws')
// const socketRouter = require('./routes/socket');
const socketRouter = require('./socket');

const app = express();

// socket of express
expressWs(app);
app.use('/socket', socketRouter);

app.use(express.static('static'));
app.get('/', (req, res) => {
  res.send('Hello, world!');
});

app.listen(3000, () => {
  console.log('Server listening on port 3000');
});

module.exports = router;