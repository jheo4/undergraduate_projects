#
# Author : Jin Heo
# Association : Ajou Univ
# Date : 14 04 2018
#

from algorithms.alternative import get_result as alternative_get_result
from algorithms.extreme_accuracy import extreme_get_result
from algorithms.background import get_background

def find_optimal(
        start_thres,
        end_thres,
        threshold_increse,
        start_alpha,
        end_alpha,
        alpha_increase,):

    prev_thres_sum = 0
    prev_alpha_sum = 0
    isOptimalAlpha = 0
    f = open("optimal.csv", "a")
    data = 0

    for alpha in range(start_alpha, end_alpha, alpha_increase):
        get_background(alpha/1000000)
        prev_thres_sum=0

        for threshold in range(start_thres, end_thres, threshold_increse):
            rec_prec = extreme_get_result(threshold)
            cur_sum = rec_prec[0] + rec_prec[1]
            print("thresh, {0}, alpha, {4}, recall, {1}, prec, {2}, sum, {3}\n".format(threshold, rec_prec[0], rec_prec[1], cur_sum, alpha))

            # reduce search space for alpha
            if(threshold == start_thres):
                if(cur_sum < prev_alpha_sum):
                    print("cur sum : ", cur_sum, "prev_sum : ", prev_alpha_sum)
                    isOptimalAlpha = 1
                    break
                else:
                    prev_alpha_sum = cur_sum

            # reduce search sapce for threshold
            if(cur_sum > prev_thres_sum):
                prev_thres_sum = cur_sum
                data = "thresh, {0}, alpha, {4}, recall, {1}, prec, {2}, sum, {3}\n".format(threshold, rec_prec[0], rec_prec[1], cur_sum, alpha)
            else :
                f.write(data)
                break

        if(isOptimalAlpha == 1):
            break

    f.close()


def autoTest():
    find_optimal(start_thres=16, end_thres=17, threshold_increse=1, start_alpha=15, end_alpha=16, alpha_increase=1)

if __name__ == '__main__':
    autoTest()


