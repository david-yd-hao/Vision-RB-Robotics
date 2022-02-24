import collections
def get_const_points(points_over_time, number):
    exist = False
    first = points_over_time[0]
    distances = [0] * len(first)
    for i in range(1, len(points_over_time)):
        next = points_over_time[i]
        for j in range(len(first)):
            shortest, _ = get_closest(first[j], next)
            distances[j] += shortest
    distances = [i/len(points_over_time) for i in distances]
    zip_iterator = zip(first, distances)

    points = dict(zip_iterator)
    pdict = {k: v for k, v in sorted(points.items(), key=lambda item: item[1])}
    results = []
    i = 0
    for k, v in pdict.items():
        results.append(k)
        i += 1
        if i == number:
            exist = True
            break
    return results, exist
def get_dist(first, second):
    return ((second[1] - first[1])**2 + (second[0] - first[0])**2)**0.5
def get_closest(point, point_list):
    shortest = get_dist(point, point_list[0])
    shortestp = point_list[0]
    for i in point_list:
        if get_dist(point, i)<= shortest:
            shortest = get_dist(point, i)
            shortestp = i
    return shortest, shortestp
def get_red_dest(frame):
    return
