import asyncio
import websockets

async def echo(websocket, path):
    async for message in websocket:
        await websocket.send("hello, it's echo: " + message)

async def main():
    async with websockets.serve(echo, "0.0.0.0", 8765):
        await asyncio.Future()  # run forever

asyncio.run(main())