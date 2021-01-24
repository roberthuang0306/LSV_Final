import sys
sys.path.append( "/home/yen-hsiang/.local/lib/python3.8/site-packages" )
import drawSvg as draw
import string

f1 = open( sys.argv[1], 'r' )
contents = list(filter(str.strip, f1.readlines()))
f1.close()
inputInfo = contents[0]
contents.remove(contents[0])
inputInfo = inputInfo.replace('\n', '').split(' ')
inputNum = int(inputInfo[-1])
X = 3*(len(contents)+5)
Y = 3*(inputNum+3)
d = draw.Drawing(X,Y)

for i in range(inputNum):
    d.append(draw.Text(str(i)+'\'',3, X-3,i*3+6-1))
    d.append(draw.Text(str(i),3, 1.5,i*3+6-1))
    d.append(draw.Line(6,i*3+6,X-6,i*3+6,stroke_width=0.2, stroke='black'))

for i, content in enumerate(contents):
    content = content.split()
    ctrl = int(content[0])
    target = int(content[1])
    d.append(draw.Line(9+i*3,ctrl*3+6, 9+i*3,target*3+6,stroke_width=0.2, stroke='black'))
    d.append(draw.Circle(9+i*3,ctrl*3+6, 0.5))
    d.append(draw.Circle(9+i*3,target*3+6, 0.75,fill='white', fill_opacity=0.0, stroke_width=0.2, stroke='black'))
    d.append(draw.Line(9+i*3-0.75,target*3+6, 9+i*3+0.75,target*3+6,stroke_width=0.2, stroke='black'))
    d.append(draw.Line(9+i*3,target*3+6-0.75, 9+i*3,target*3+6+0.75,stroke_width=0.2, stroke='black'))

d.setPixelScale(1080/Y)  # Set number of pixels per geometry unit
#d.setRenderSize(400,200)  # Alternative to setPixelScale
d.saveSvg(sys.argv[2]+".svg")
d.savePng(sys.argv[2]+".png")

# Display in iPython notebook
d  # Display as SVG