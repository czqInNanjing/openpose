from __future__ import unicode_literals, print_function, division
from io import open
import random
import numpy
import time
import torch
import torch.nn as nn
from torch.autograd import Variable
from ClassifierParam import *


class GRU(nn.Module):
    def __init__(self, input_size, hidden_size, output_size, num_layers=1):
        super(GRU, self).__init__()
        self.hidden_size = hidden_size
        self.num_layers = num_layers
        self.gru = nn.GRU(input_size, hidden_size, num_layers)
        self.out = nn.Linear(hidden_size, output_size)
        self.softmax = nn.LogSoftmax(dim=1)

    def forward(self, input, hidden):
        # [seq_len, batch_size, input_size]
        input = input.view(1, 1, -1)
        output, hidden = self.gru(input, hidden)
        output = self.softmax(self.out(output[0]))
        return output, hidden

    def initHidden(self):
        return Variable(torch.zeros(self.num_layers, 1, self.hidden_size))


def initModel(model_path):
    model = GRU(input_size, hidden_size, output_size, num_layers)
    now_best_path = model_path
    print('start loading model from {}'.format(now_best_path))
    resume_model = torch.load(now_best_path)
    if 'loss' in resume_model:
        for k, v in resume_model.items():
            if k != 'dict':
                print('k: {} v: {}'.format(k, v))
        model.load_state_dict(resume_model['dict'])
    else:
        model.load_state_dict(torch.load(now_best_path))
    return model


def outputToAction(output):
    all = output.data.size()[1]
    top_n, top_i = output.data.topk(all)
    action_top_list = []
    pro_top_list = []
    for i in range(all):
        action_top_list.append(actions_list[top_i[0][i]])
        pro_top_list.append(numpy.exp(top_n[0][i]))
    return action_top_list, pro_top_list


def forwardModel(model, pose):
    parts = pose.strip().split(' ')
    one_frame = []
    for part in parts:  # should be 18
        axis = part.split(',')
        one_frame.append([float(axis[0]), float(axis[1])])
    # throw away axis that have no relation to action recognition
    pose = one_frame[1:14]
    frame = Variable(torch.FloatTensor([pose])).view(-1, 1, 26)
    hidden = model.initHidden()
    output, _ = model(frame[0], hidden)
    return outputToAction(output)
