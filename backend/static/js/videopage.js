const random = (min, max) => +(Math.random() * (max - min) + min).toFixed(0)
// 初始化 video-js 元素
videojs(document.querySelector('video-js'));
let isVideoPaused = false;

let allNumber = 0;
let showNumber = 0;

const m = Danmuku.create({
    container: document.getElementById('barrage-area'),
    height: 35, // 轨道高度
    rowGap: 10, // 两个弹幕的横向间隔（速度不一样，后面的可能会追上）
    direction: 'right', // 弹幕的方向 left or right
    times: [8, 8], // 弹幕动画的时间取值范围
    limit: 150, // 能够渲染的最大数量
    interval: 1,
    hooks: {  // 这里的生命周期，特殊弹幕和普通弹幕都会调用
        // 弹幕创建时
        barrageCreate(barrage, node) {
            if (barrage.isSpecial) return

            if (typeof barrage.data !== 'string') {
                node.style.border = '3px solid #fff'
                node.textContent = barrage.data.content
            } else {
                node.textContent = barrage.data
            }

            node.classList.add('barrage')
            // node.style.color = `rgba(${random(0, 255)}, ${random(0, 255)}, ${random(0, 255)})`
            node.style.color = 'rgba(255, 255, 255, 0.9)'
        },

        // // 弹幕开始移动时
        barrageMove(barrage, node) {
            console.log("barrage:", barrage);
            console.log("node", node)
        },

        // // 弹幕节点移除视图时
        barrageRemove(barrage, node) {
        },

        barrageDestroy(barrage, node) {
        },

        send() {
        },

        ended() {
        },

        capacityWarning(manager) {
        },

        willRender(manager, barrage, isSpecial) {
        },

        render() {
        },

        start() {
        },

        stop() {
        },

        clear() {
        },

        setOptions() {
        },

        show() {
        },

        hidden() {
        },
    }
})


// 监听当前视频播放时间
var myplayer = videojs('my-player');
var timeDisplay = myplayer.currentTime();
// console.log(timeDisplay)
myplayer.on("timeupdate", mytime);

function mytime() {
    timeDisplay = Math.floor(myplayer.currentTime());
}

// 监听当前视频是否暂停
myplayer.on("pause", () => {
    isVideoPaused = true;
})

// 监听当前视频是否播放
myplayer.on("play", () => {
    isVideoPaused = false;
    // m.clear();
    // m.start();

})

// 监听视频进度条是否被拖动或点击
myplayer.on('seeking', () => {
    // m.clear();
    // m.start();
});

myplayer.on('seeked', () => {
    m.clear();
    m.start();
});



var barrageDataRequest = [];
const timeline = m.use(Danmuku.Timeline, { forceRender: true })

function barrageReload() {
    // m.clear();
    timeline.timeStore = [];
    console.log("timeline has been reloaded!");
    console.log(timeline);

    for (let i = 0; i < barrageDataRequest.length; i++) {
        console.log("barrageDataRequest.length:", barrageDataRequest.length)

        if (barrageDataRequest.length === 0) {
            console.log("barrageDataRequest is NULL!!!")
        }

        if (barrageDataRequest[i].barrageText === undefined) {
            continue;
        } else {
            timeline.add(barrageDataRequest[i].barrageTime, barrageDataRequest[i].barrageText, {
                // 弹幕渲染到页面上时
                append(barrage, node) {
                    node.onmouseenter = e => {
                        barrage.pause()
                        node.classList.add('active')
                    }
                    node.onmouseleave = e => {
                        barrage.resume()
                        node.classList.remove('active')
                    }
                    node.onclick = e => {
                        barrage.destroy()
                    }
                },
            })
        }


    }
    console.log("timeline:")
    console.log(timeline);
    m.start();
}

let videoId = document.getElementById("videoId").value;

function request() {
    $.ajax({
        url: "https://engage-2023.com/barrage?vid=" + videoId,
        method: 'GET',
        success: (data) => {
            console.log(data);
            barrageDataRequest = data.barrages;
            barrageReload();
        },
        error: (error) => {
            alert('Error fetching barrages: ' + error.statusText);
        }
    });
}

request();
barrageReload();
m.start();

setInterval(() => {
    if (isVideoPaused === false) {
        timeline.emit(timeDisplay)
    }
}, 1000)

const WS_URL = "wss://engage-2023.com/wss";
// const ws = new WebSocket('ws://20.222.213.210:8081');
let ws;
function connectws() {
    ws = new WebSocket(WS_URL);
    // ws = new ReconnectingWebSocket(WS_URL);

    ws.onopen = function () {
        console.log('Connected to WebSocket server');
    };

    ws.onmessage = (event) => {
        const { data = "" } = event || {};
        if (data.startsWith("barrage updated")) {
            request();
        }
    };

    ws.onclose = function (e) {
        console.log('Disconnected from WebSocket server', e.reason);
        setTimeout(connectws, 5000);
    };

    ws.onerror = function (err) {
        console.error('WebSocket encountered an error', err.message);
        ws.close();
    }
}

connectws();

let heartbeatInterval = null;
const heartbeatRate = 5000;

const sendButton = document.getElementById('send');
const messageInput = document.getElementById('message');

sendButton.onclick = function () {
    const message = messageInput.value;

    // 显示到屏幕上
    m.send({ content: message }, {}, true)

    // check ws connection before sending
    if (ws.readyState != WebSocket.OPEN) {
        alert('WebSocket is not connected. Reconnecting...');
        connectws();
        return;
    }

    // 构建JSON对象
    const payload = {
        vid: videoId,
        barrageText: message,
        barrageTime: timeDisplay
    };

    ws.send(JSON.stringify(payload));
    console.log('send: ' + JSON.stringify(payload));
    messageInput.value = '';
};

