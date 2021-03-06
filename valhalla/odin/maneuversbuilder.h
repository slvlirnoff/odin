#ifndef VALHALLA_ODIN_MANEUVERSBUILDER_H_
#define VALHALLA_ODIN_MANEUVERSBUILDER_H_

#include <list>

#include <valhalla/proto/trippath.pb.h>
#include <valhalla/proto/directions_options.pb.h>
#include <valhalla/odin/enhancedtrippath.h>
#include <valhalla/odin/maneuver.h>

namespace valhalla {
namespace odin {

/**
 * Builds the maneuver list based on the specified directions options and
 * enhanced trip path.
 */
class ManeuversBuilder {
 public:

  /**
   * Constructor that assigns the specified directions options and trip path.
   *
   * @param directions_options The directions options such as: units and
   *                           language.
   * @param trip_path The trip path - list of nodes, edges, attributes and shape.
   */
  ManeuversBuilder(const DirectionsOptions& directions_options,
                   EnhancedTripPath* trip_path);

  std::list<Maneuver> Build();

 protected:
  std::list<Maneuver> Produce();

  void Combine(std::list<Maneuver>& maneuvers);

  std::list<Maneuver>::iterator CollapseTransitConnectionStartManeuver(
      std::list<Maneuver>& maneuvers,
      std::list<Maneuver>::iterator curr_man,
      std::list<Maneuver>::iterator next_man);

  std::list<Maneuver>::iterator CollapseTransitConnectionDestinationManeuver(
      std::list<Maneuver>& maneuvers,
      std::list<Maneuver>::iterator curr_man,
      std::list<Maneuver>::iterator next_man);

  std::list<Maneuver>::iterator CombineInternalManeuver(
      std::list<Maneuver>& maneuvers, std::list<Maneuver>::iterator prev_man,
      std::list<Maneuver>::iterator curr_man,
      std::list<Maneuver>::iterator next_man, bool start_man);

  std::list<Maneuver>::iterator CombineTurnChannelManeuver(
      std::list<Maneuver>& maneuvers, std::list<Maneuver>::iterator prev_man,
      std::list<Maneuver>::iterator curr_man,
      std::list<Maneuver>::iterator next_man, bool start_man);

  std::list<Maneuver>::iterator CombineSameNameStraightManeuver(
      std::list<Maneuver>& maneuvers, std::list<Maneuver>::iterator curr_man,
      std::list<Maneuver>::iterator next_man);

  void CountAndSortExitSigns(std::list<Maneuver>& maneuvers);

  void CreateDestinationManeuver(Maneuver& maneuver);

  void CreateStartManeuver(Maneuver& maneuver);

  void InitializeManeuver(Maneuver& maneuver, int node_index);

  void UpdateManeuver(Maneuver& maneuver, int node_index);

  void FinalizeManeuver(Maneuver& maneuver, int node_index);

  void SetManeuverType(Maneuver& maneuver);

  void SetSimpleDirectionalManeuverType(Maneuver& maneuver,
                                        EnhancedTripPath_Edge* prev_edge,
                                        EnhancedTripPath_Edge* curr_edge);

  TripDirections_Maneuver_CardinalDirection DetermineCardinalDirection(
      uint32_t heading);

  bool CanManeuverIncludePrevEdge(Maneuver& maneuver, int node_index);

  bool IncludeUnnamedPrevEdge(int node_index, EnhancedTripPath_Edge* prev_edge,
                              EnhancedTripPath_Edge* curr_edge) const;

  bool IsFork(int node_index, EnhancedTripPath_Edge* prev_edge,
              EnhancedTripPath_Edge* curr_edge) const;

  bool IsTee(int node_index, EnhancedTripPath_Edge* prev_edge,
             EnhancedTripPath_Edge* curr_edge) const;

  bool IsLeftPencilPointUturn(int node_index, EnhancedTripPath_Edge* prev_edge,
                              EnhancedTripPath_Edge* curr_edge) const;

  bool IsRightPencilPointUturn(int node_index, EnhancedTripPath_Edge* prev_edge,
                               EnhancedTripPath_Edge* curr_edge) const;

  bool IsIntersectingForwardEdge(int node_index,
                                 EnhancedTripPath_Edge* prev_edge,
                                 EnhancedTripPath_Edge* curr_edge) const;

  void DetermineRelativeDirection(Maneuver& maneuver);

  static Maneuver::RelativeDirection DetermineRelativeDirection(
      uint32_t turn_degree);

  bool UsableInternalIntersectionName(Maneuver& maneuver, int node_index) const;

  void UpdateInternalTurnCount(Maneuver& maneuver, int node_index) const;

  /**
   * Returns the speed based on the specified travel mode.
   *
   * @param travel_mode The current specified travel mode.
   * @param edge_speed The speed of the current edge - used for driving mode.
   *
   * @return the speed based on the specified travel mode.
   */
  float GetSpeed(TripPath_TravelMode travel_mode, float edge_speed) const;

  const DirectionsOptions& directions_options_;
  EnhancedTripPath* trip_path_;

};

}
}

#endif  // VALHALLA_ODIN_MANEUVERSBUILDER_H_
