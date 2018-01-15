import shutil

import torch
# Dataset of KTH
actions_list = ['handwaving', 'walking', 'bg', 'boxing', 'handclapping', 'running', 'jogging']
n_actions = len(actions_list)
action_folder = 'data/action/'
input_x = 160.0
input_y = 120.0

# Dataset of weizmann
# actions_list = ["bend", "jack", "jump", "pjump", "run", "side", "skip", "walk", "wave1", "wave2"]
# n_actions = len(actions_list)
# action_folder = 'data/Weizmann/'
# input_x = 180.0
# input_y = 144.0


# fps of the original data
data_fps = 25
# fps we prefer when testing
goal_fps = 5

example_each_action = 500
save_path = 'models/ver2/mode3_with_scale.pth.tar'
best_path = 'models/ver2/mode3_with_scale.pth.tar'
use_cuda = torch.cuda.is_available()
# Train parameters
train2test = 4
input_size = 26
hidden_size = 128
output_size = n_actions
num_layers = 3

action_frames_nums = 40000
print_each = 500

# Data Augmentation

# Random start part of the whole action, should be 0 - 1, when set to 0, start from the beginning
# start_frame = int(random.random(0, start_ratio)*end_frame)
start_ratio = 0.6

# Scale the input data
use_scale = True
scale_list = [0.5, 0.75, 1.0, 1.5, 2.0]



# Description of best_ver4
# layers = 3 actions = all 7     train epochs = 40000  lowest loss = 0.17 start_ratio = 0.8

# Description of best_ver5
# layers = 3 actions = all 7     train epochs = 50000  lowest_loss = 0.17 start_ratio = 0.6


