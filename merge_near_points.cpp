#include "geometry.hpp"
#include "bg_operators.hpp"

#include <vector>
using std::vector;

#include <utility>
using std::pair;

// Points that are very close to each other, probably because of a rounding
// error, are merged together to a single location.
size_t merge_near_points(std::map<point_type_fp, point_type_fp>& points, const coordinate_type_fp distance) {
  // Merge points that are near one another.  This doesn't do a
  // great job but it's fast enough.
  size_t points_merged = 0;
  const auto distance_2 = distance * distance;
  for (auto i = points.begin(); i != points.end(); i++) {
    for (auto j = i;
         j != points.upper_bound(point_type_fp(i->second.x()+distance,
                                               i->second.y()+distance));
         j++) {
      if (!bg::equals(j->second, i->second) &&
          bg::comparable_distance(i->second, j->second) <= distance_2) {
        points_merged++;
        j->second = i->second;
      }
    }
  }
  return points_merged;
}

size_t merge_near_points(vector<pair<linestring_type_fp, bool>>& mls, const coordinate_type_fp distance) {
  std::map<point_type_fp, point_type_fp> points;
  for (const auto& ls_and_allow_reversal : mls) {
    for (const auto& point : ls_and_allow_reversal.first) {
      points[point] = point;
    }
  }
  size_t points_merged = merge_near_points(points, distance);
  if (points_merged > 0) {
    for (auto& ls_and_allow_reversal : mls) {
      for (auto& point : ls_and_allow_reversal.first) {
        point = points[point];
      }
    }
  }
  return points_merged;
}

size_t merge_near_points(multi_linestring_type_fp& mls, const coordinate_type_fp distance) {
  std::map<point_type_fp, point_type_fp> points;
  for (const auto& ls : mls) {
    for (const auto& point : ls) {
      points[point] = point;
    }
  }
  size_t points_merged = merge_near_points(points, distance);
  if (points_merged > 0) {
    for (auto& ls : mls) {
      for (auto& point : ls) {
        point = points[point];
      }
    }
  }
  return points_merged;
}
