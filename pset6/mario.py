def main():
    print("START")
    while True:
        print("Height: ")
        n = input()
        if n.isdigit() and int(n) > 0 and int(n) < 23:
            build_Pyramid(int(n))
            break
        
    
def build_Pyramid(n):
    for rows in range(n):
        spaces = n - rows - 1
        for spaces in reversed(range(spaces)):
            print (" " , end = "")
        for hases in range(rows + 1):
            print ("#" , end = "")
        print(" ",end = "")
        print(" ",end = "")
        for hases in range(rows + 1):
            print ("#" , end = "")    
        print()
        
if __name__ == "__main__":
    main()
    