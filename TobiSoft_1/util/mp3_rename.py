import os

increment = 0

for filename in os.listdir('.'):
    if filename.endswith(".mp3"): 
        increment += 1
		
        print("here's one: " + os.path.join(filename))

        if increment > 9:
            outname = '00' + str(increment) + '.mp3'
            os.rename(filename, outname)
        else:
            outname = '000' + str(increment) + '.mp3'
            os.rename(filename, outname)

    else:
        continue