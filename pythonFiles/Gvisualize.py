import sys
sys.path.append( "/home/yen-hsiang/.local/lib/python3.8/site-packages" )
import drawSvg as draw
import numpy as np

f1 = open( sys.argv[1], 'r' )
contents = list(filter(str.strip, f1.readlines()))
f1.close()
inputInfo = contents[0]
contents.remove(contents[0])
inputInfo = inputInfo.replace('\n', '').split(' ')
inputNum = int(inputInfo[-1])
id2Coord = {}
id2Neighbor = {}
coords = []
ids = []
for content in contents:
    content = content.split(' ')
    content = list(filter(str.strip, content))

    _id = int(content[0])
    ids.append(_id)
    coord = content[1].replace('(', '').replace(')', '').replace('\n', '')
    neighbors = content[2].replace('{', '').replace('}', '').replace('\n', '')
    coord = list(map(int, (coord.split(','))))
    coords.append(coord);
    neighbors = list(map(int, (neighbors.split(','))))
    id2Coord[_id] = coord
    id2Neighbor[_id] = neighbors
    (_id, coord, neighbors)
coords = np.array(coords)
Maxx = max(coords[:,0])
minx = min(coords[:,0])
Maxy = max(coords[:,1])
miny = min(coords[:,1])
X = 3*(Maxx+3)
Y = 3*(Maxy+3)
d = draw.Drawing(X,Y)

for _id in ids:
        [x,y] = id2Coord[_id]
        for neighbor in id2Neighbor[_id]:
            [_x, _y] = id2Coord[neighbor]
            d.append(draw.Line(3*x+3,3*y+3,3*_x+3,3*_y+3,stroke_width=0.1, stroke='black'))
for _id in ids:
        [x,y] = id2Coord[_id]
        d.append(draw.Circle(3*x+3,3*y+3, 0.75 ,fill='white', fill_opacity=1, stroke_width=0.1, stroke='black'))
        d.append(draw.Text(str(_id),0.75, 3*x+2.75,3*y+2.75))
# for i in range(inputNum):
#     d.append(draw.Text(str(i),3, 1.5,i*3+6-1))
#     d.append(draw.Line(6,i*3+6,X-3,i*3+6,stroke_width=Y/240, stroke='black'))

# for i, content in enumerate(contents):
#     content = content.split()
#     ctrl = int(content[0])
#     target = int(content[1])
#     d.append(draw.Line(9+i*3,ctrl*3+6, 9+i*3,target*3+6,stroke_width=Y/180, stroke='black'))
#     d.append(draw.Circle(9+i*3,ctrl*3+6, Y/100))
#     d.append(draw.Circle(9+i*3,target*3+6, Y/60,fill='white', fill_opacity=0.0, stroke_width=Y/180, stroke='black'))
#     d.append(draw.Line(9+i*3-Y/60,target*3+6, 9+i*3+Y/60,target*3+6,stroke_width=Y/180, stroke='black'))
#     d.append(draw.Line(9+i*3,target*3+6-Y/60, 9+i*3,target*3+6+Y/60,stroke_width=Y/180, stroke='black'))

d.setPixelScale(1080/Y)  # Set number of pixels per geometry unit
#d.setRenderSize(400,200)  # Alternative to setPixelScale
d.saveSvg(sys.argv[2]+".svg")
d.savePng(sys.argv[2]+".png")

# Display in iPython notebook
d  # Display as SVG