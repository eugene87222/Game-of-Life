import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

def randomBoard(N): 
    board = np.random.choice([0, 1], N * N, p=[0.2, 0.8]).reshape(N, N)
    full = np.ones((N+2, N+2))
    for i in range(1, N+1):
        full[i, 1:N+1] = board[i-1]
    return full


def update(frameNum, img, board, N):
    new_board = board.copy()
    for i in range(1, N+1):
        for j in range(1, N+1):
            neighbor = board[i-1:i+2, j-1:j+2].ravel()
            unique, counts = np.unique(neighbor, return_counts=True)
            counts = dict(zip(unique, counts))
            if board[i][j] == 0:
                if counts[0] < 3 or counts[0] > 4:
                    new_board[i][j] = 1
            else:
                if 0 in counts and counts[0] == 3:
                    new_board[i][j] = 0
    img.set_data(new_board)
    board[:] = new_board[:]
    return img, 


if __name__ == '__main__':
    # 0 for alive
    # 1 for dead
    N = 50
    BOARD = randomBoard(N)
    fig, ax = plt.subplots()
    img = ax.imshow(BOARD[1:N+1, 1:N+1], cmap='gray')
    ani = animation.FuncAnimation(fig, update, fargs=(img, BOARD, N,), frames=10, interval=50)
    plt.show()