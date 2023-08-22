// console.log(barragedata);
const random = (min, max) => +(Math.random() * (max - min) + min).toFixed(0)
// const testData = [
//     '陈涛',
//     'Imtaotao',
//     'Supertaotao',
//     'I can see it',
//     'Do you feel alright?',
//     'Yes, I feel wonderfull tonight',
//     'Tell me someday we\'ll get together'
// ]

// 测试代码
const m = Danmuku.create({
    container: document.getElementById('container'),
    height: 35, // 轨道高度
    rowGap: 10, // 两个弹幕的横向间隔（速度不一样，后面的可能会追上）
    direction: 'right', // 弹幕的方向 left or right
    times: [4, 8], // 弹幕动画的时间取值范围
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
            node.style.color = `rgba(${random(0, 255)}, ${random(0, 255)}, ${random(0, 255)})`
        },

        // 弹幕开始移动时
        barrageMove(barrage, node) {
            // 查看当前这个弹幕是否被修正过停留在视图的时长
            // console.log(barrage.isChangeDuration)
            node.style.opacity = opacity.value / 100
        },

        // // 弹幕节点移除视图时
        barrageRemove(barrage, node) {
        },

        barrageDestroy(barrage, node) {
            allNumber.textContent = m.length
            showNumber.textContent = m.showLength
        },

        // send() {
        //     allNumber.textContent = m.length
        // },

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

// m.start()

// // 模拟从服务器得到的数据，不停的发送
// setInterval(() => m.send(testData, {
//     // 弹幕渲染到页面上时
//     append(barrage, node) {
//         node.onmouseenter = e => {
//             barrage.pause()
//             node.classList.add('active')
//         }
//         node.onmouseleave = e => {
//             barrage.resume()
//             node.classList.remove('active')
//         }
//         node.onclick = e => {
//             barrage.destroy()
//         }
//     },
//     show() {
//         console.log('show')
//     },
// }), 250)

var timeDisplay = 0;
var video = document.getElementById('video');
//使用事件监听方式捕捉事件
video.addEventListener("timeupdate", function () {
    // var timeDisplay;
    //用秒数来显示当前播放进度
    timeDisplay = Math.floor(video.currentTime);
    // console.log(Math.floor(video.currentTime))
    //当视频播放到 4s的时候做处理

}, false);


const count = 30
const repeat = 2
// const data = [
//     {barrageText: "测试1", barrageTime: 1},

// ]
const timeline = m.use(Danmuku.Timeline, { forceRender: true })
for (let i = 0; i < barragedata.length; i++) {
    // let data;
    // if (i < 10) {
    //     data = new Array(i * repeat).fill(String(i).repeat(5))
    // } else {
    //     data = new Array(String(count - i) * repeat).fill(String(i).repeat(5))
    // }
    // console.log(i);
    // console.log(barragedata[i].barrageTime);
    // console.log(barragedata[i].barrageText);
    timeline.add(barragedata[i].barrageTime, barragedata[i].barrageText, {
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
m.start()


// let i = 0
// setInterval(() => {
//     timeline.emit(i)
//     time.innerHTML = i
//     i++
// }, 1000)
setInterval(() => {
    timeline.emit(timeDisplay)
    time.innerHTML = timeDisplay
    // i++
}, 1000)

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

// 发送弹幕
send.onclick = e => {
    m.send({ content: barrageText.value }, {}, true)
    barrageText.value = ''
}

// 发送高级弹幕
sendSpecial.onclick = e => {
    const text = specialBarrage.value
    const directions = ['right', 'left', 'none']
    specialBarrage.value = ''

    m.sendSpecial({
        duration: random(3, 5),
        direction: directions[random(0, 2)],
        position(barrage) {
            return {
                x: m.containerWidth / 2,
                y: 100,
            }
        },
        hooks: {
            create(barrage, node) {
                node.textContent = `高级弹幕--${text}`
                node.classList.add('special-barrage')
            },
            destroy() {
                console.log('高级弹幕销毁')
            }
        }
    })
}

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