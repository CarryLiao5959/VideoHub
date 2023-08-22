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
            node.style.color = 'rgba(255, 255, 255, 0.5)'
        },

        // // 弹幕开始移动时
        // barrageMove(barrage, node) {
        //     // 查看当前这个弹幕是否被修正过停留在视图的时长
        //     // console.log(barrage.isChangeDuration)
        //     node.style.opacity = opacity.value / 100
        // },

        // // 弹幕节点移除视图时
        barrageRemove(barrage, node) {
        },

        // barrageDestroy(barrage, node) {
        //     allNumber.textContent = m.length
        //     showNumber.textContent = m.showLength
        // },

        send() {
            // allNumber.textContent = m.length
        },

        ended() {
            console.log('end')
        },

        capacityWarning(manager) {
            console.error('capacityWarning 钩子被调用')
        },

        willRender(manager, barrage, isSpecial) {
            if (!isSpecial) return
            console.log(barrage, isSpecial)
        },

        // render() {
        //     allNumber.textContent = m.length
        //     showNumber.textContent = m.showLength
        // },

        // start() {
        //     start.classList.add('select')
        //     stopBtn.classList.remove('select')
        // },

        // stop() {
        //     start.classList.remove('select')
        //     stopBtn.classList.add('select')
        // },

        clear() {
            allNumber.textContent = m.length
            showNumber.textContent = m.showLength
        },

        setOptions() {
            console.log('setOptions')
        },

        show() {
            show.classList.add('select')
            hidden.classList.remove('select')
        },

        hidden() {
            show.classList.remove('select')
            hidden.classList.add('select')
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
})

// const count = 30
// const repeat = 2

var barrageDataRequest = [];
const timeline = m.use(Danmuku.Timeline, { forceRender: true })

function barrageReload() {
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
    m.start()
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

// const ws = new WebSocket('ws://20.222.213.210:8081');
const ws = new WebSocket("wss://engage-2023.com/wss");
ws.onopen = function () {
    console.log('Connected to WebSocket server');
    // const vid_info={"vid":videoId};
    // ws.send(JSON.stringify(vid_info));
    // console.log('send: ' + JSON.stringify(vid_info));
};

ws.onmessage = (event) => {
    const { data = ""} = event || {};
    if (data.startsWith("barrage updated")) {
        request();
    }
};

ws.onclose = function () {
    console.log('Disconnected from WebSocket server');
};

const sendButton = document.getElementById('send');
const messageInput = document.getElementById('message');

sendButton.onclick = function () {
    const message = messageInput.value;

    // 显示到屏幕上
    m.send({ content: message }, {}, true)

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

// 定义初始值
height.value = m.opts.height
limit.value = m.opts.limit
rowGap.value = m.opts.rowGap
renderTime.value = m.opts.interval
minTime.value = m.opts.times[0]
maxTime.value = m.opts.times[1]

const setDirection = direction => {
    if (direction === 'left') {
        left.classList.add('select')
        right.classList.remove('select')
    } else {
        left.classList.remove('select')
        right.classList.add('select')
    }
}
setDirection(m.opts.direction)

const input = (node, cb) => {
    node.oninput = e => {
        const val = Number(e.currentTarget.value)
        if (typeof val === 'number' && !isNaN(val)) {
            cb(val)
        }
    }
}

// 轨道高度
input(height, val => {
    m.setOptions({ height: val })
})

// limit
input(limit, val => {
    m.setOptions({ limit: val })
})

// rowgap
input(rowGap, val => {
    m.setOptions({ rowGap: val })
})

// render time
input(renderTime, val => {
    m.setOptions({ interval: val })
})

// 隐藏显示
show.onclick = e => {
    m.show()
}

hidden.onclick = e => {
    m.hidden()
}

// direction
left.onclick = e => {
    setDirection('left')
    m.setOptions({ direction: 'left' })
}

right.onclick = e => {
    setDirection('right')
    m.setOptions({ direction: 'right' })
}

// 启动
start.onclick = e => {
    m.start()
}

stopBtn.onclick = e => {
    m.stop()
}

// times
input(minTime, min => {
    const max = m.opts.times[1]
    m.setOptions({ times: [min, max] })
})

input(maxTime, max => {
    const min = m.opts.times[0]
    m.setOptions({ times: [min, max] })
})


// 清空弹幕
clear.onclick = e => {
    m.clear()
}

// 透明度
input(opacity, val => {
    val /= 100
    m.each(barrage => {
        barrage.node.style.opacity = val
    })
})

// 显示区域
const toggle = node => {
    [allArea, topArea, bottomArea].forEach(v => {
        if (v === node) {
            v.classList.add('select')
        } else {
            v.classList.remove('select')
        }
    })
}

// 全部半部分区域
allArea.onclick = e => {
    m.container.style.height = '100vh'
    m.container.style.transform = 'translateY(0)'
    m.resize()
    toggle(allArea)
}

// 上半部分区域
topArea.onclick = e => {
    m.container.style.height = '50vh'
    m.container.style.transform = 'translateY(0)'
    m.resize()
    toggle(topArea)
}

// 下半部分区域
bottomArea.onclick = e => {
    m.container.style.height = '50vh'
    m.container.style.transform = 'translateY(100%)'
    m.resize()
    toggle(bottomArea)
}

// 实时响应
responsive.onclick = e => {
    m.setOptions({
        rowGap: -1,
        limit: Infinity,
    })
    rowGap.value = -1
    limit.value = Infinity
}