import json

def get_video_url_by_vid(vid):
    with open('/home/engage/github_projects/socket/backend/RESTful/videos.json', 'r') as file:
        data = json.load(file)
        for video in data["videos"]:
            if video["vid"] == vid:
                return video["url"]
    return None