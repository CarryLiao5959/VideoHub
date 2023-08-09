import React, { useEffect, useState } from 'react';
import BulletScreen, { StyledBullet } from 'rc-bullets';
import './App.css';

const headUrl = '../public/logo192.png';
export default function App() {
  // 弹幕屏幕
  const [screen, setScreen] = useState(null);
  // 弹幕内容
  const [bullet, setBullet] = useState('');
  useEffect(() => {
    // 给页面中某个元素初始化弹幕屏幕，一般为一个大区块。此处的配置项全局生效
    let s = new BulletScreen('.screen', { duration: 20 });
    // or
    // let s=new BulletScreen(document.querySelector('.screen));
    setScreen(s);
  }, []);
  // 弹幕内容输入事件处理
  const handleChange = ({ target: { value } }) => {
    setBullet(value);
  };
  // 发送弹幕
  const handleSend = () => {
    if (bullet) {
      // push 纯文本
      screen.push(bullet);
      // or 使用 StyledBullet

      screen.push(
        <StyledBullet
          head={headUrl}
          msg={bullet}
          backgroundColor={'#fff'}
          size='large'
        />
      );
      // or 还可以这样使用，效果等同使用 StyledBullet 组件
      // screen.push({msg:bullet,head:headUrl,color:"#eee" size="large" backgroundColor:"rgba(2,2,2,.3)"})
    }
  };
  return (
    <main>
      <div className="screen" style={{ width: '100vw', height: '80vh' }}></div>
      <div class='top-tips'>
        如果需要实时响应发送的弹幕，可以设置 limit 为 Infinity, rowGap 为 -1。但需要特别小心弹幕的存储容量和渲染数量过多，导致的内存飙升
      </div>

      <div class='operation-panel'>
        <a class='icon' alt='' href='../public/logo192.png'></a>
        <div class='item tips'>
          点击弹幕会消失
        </div>

        <div class='item'>
          <span>透明度: </span>
          <input type='range' class='input-range' min='0' max='100' value='100' id='opacity' />
        </div>

        <div class='item'>
          <span>limit: </span>
          <input type='text' class='input' placeholder='最大渲染弹幕数' id='limit' />
        </div>

        <div class='item'>
          <span>rowGap: </span>
          <input type='text' class='input' placeholder='同轨道弹幕间距' id='rowGap' />
        </div>

        <div class='item'>
          <span>弹道高度: </span>
          <input type='text' class='input' placeholder='轨道的高度' id='height' />
        </div>

        <div class='item'>
          <span>渲染频率(s): </span>
          <input type='text' class='input' placeholder='渲染间隔时间' id='renderTime' />
        </div>

        <div class='item'>
          <span>显示区域: </span>
          <div class='btns'>
            <div class='button small-button select' id='allArea'>全</div>
            <div class='button small-button margin' id='topArea'>上</div>
            <div class='button small-button margin' id='bottomArea'>下</div>
          </div>
        </div>

        <div class='item'>
          <span>显示/隐藏: </span>
          <div class='btns'>
            <div class='button select' id='show'>显示</div>
            <div class='button margin' id='hidden'>隐藏</div>
          </div>
        </div>

        <div class='item'>
          <span>方向: </span>
          <div class='btns'>
            <div class='button select' id='left'>left</div>
            <div class='button margin' id='right'>right</div>
          </div>
        </div>

        <div class='item'>
          <span>启动: </span>
          <div class='btns'>
            <div class='button' id='start'>start</div>
            <div class='button margin' id='stopBtn'>stop</div>
          </div>
        </div>

        <div class='item'>
          <span>时间范围: </span>
          <div class='btns'>
            <input class='small-input' placeholder='min' id='minTime' />
            <input class='small-input margin' placeholder='max' id='maxTime' />
          </div>
        </div>

        {/* <div class='item'>
          <div class='button' id='send'>发送</div>
          <input class='input' type='text' placeholder='发送弹幕' id='barrageText' />
        </div> */}

        {/* <div class='item'>
          <div class='button' id='sendSpecial'>发送</div>
          <input class='input' type='text' placeholder='发送高级弹幕' id='specialBarrage' />
        </div> */}

        {/* <div class='item'>
          <div class='button' style='width: 80px;' id='responsive'>实时响应</div>
          <div class='button margin' id='clear'>清空</div>
        </div> */}

        {/* <div class='item'>
          <span>视图渲染弹幕数量: </span>
          <div class='button margin' id='showNumber'>0</div>
        </div> */}

        {/* <div class='item'>
          <span>现有总弹幕数量: </span>
          <div class='button margin' id='allNumber'>0</div>
        </div> */}

      </div>

      <div id='container'>
        <div class="videoares">
          <video id="video" controls autoplay="autoplay" muted>
            <source src="./assets/videos/lost_game.mp4" type="video/mp4" />
            Your browser does not support the video tag.
          </video>
        </div>
      </div>

      <input value={bullet} onChange={handleChange} />
      <button onClick={handleSend}>发送</button>
    </main>
  );
}
