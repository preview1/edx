import nltk
from nltk.tokenize import TweetTokenizer

class Analyzer():
    """Implements sentiment analysis."""

    def load(self, dictionary):
        
        self.dictionary = []                            # create an empty list
        with open(dictionary) as lines:
            for line in lines:
                if line.startswith(';') != True:        # skip comment lines
                    line = line.strip()
                    line.strip('\n')
                    if not line.strip():                # skip empty lines
                        continue
                    self.dictionary.append(line)
                    
        return  self.dictionary          

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        
        self.positives = self.load(positives)
        self.negatives = self.load(negatives)

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        
        score = 0
        tokenizer = TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        for p in self.positives:
            for token in tokens:
                if token == p:
                    score += 1
        for p in self.negatives:
            for token in tokens:
                if token == p:
                    score -= 1

        return score