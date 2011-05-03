
class Picker() :
    def __init__( self, d ) :
        self.dict = d
        self.list = []
        #self.preeditList = []
    def set( self, keys, preeditList ) :
        self.list = []
        #print keys
        for i in range( len( keys ) ) :
            key = keys[i]
            preeditString = preeditList[i]
            wordList = self.dict[key]
            self.list.append( [ key, wordList, 0, preeditString ] )
    def pick( self ) :
        highestFreq = -1
        highestIndex = -1
        for i in range( len( self.list ) ) :
            l = self.list[i]
            #key = l[0]
            wordList = l[1]
            index = l[2]
            if index < len( wordList ) :
                record = wordList[index]
                #word = record[0]
                freq = record[1]
                if freq > highestFreq :
                    highestIndex = i
                    highestFreq = freq
                #print key, word, freq
        if highestIndex >= 0 :
            l = self.list[highestIndex]
            key = l[0]
            wordList = l[1]
            index = l[2]
            preeditString = l[3]
            record = wordList[index]
            word = record[0]
            freq = record[1]
            l[2] += 1
            return key, word, freq, preeditString
        else :
            return None, None, 0, None


