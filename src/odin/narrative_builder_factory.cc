#include <boost/property_tree/ptree.hpp>

#include <valhalla/midgard/util.h>

#include "proto/directions_options.pb.h"
#include "odin/narrative_builder_factory.h"
#include "odin/narrativebuilder.h"
#include "odin/enhancedtrippath.h"
#include "odin/util.h"

namespace valhalla {
namespace odin {

std::unique_ptr<NarrativeBuilder> NarrativeBuilderFactory::Create(
    const DirectionsOptions& directions_options,
    const EnhancedTripPath* trip_path) {

  // Get the locale dictionary
  const auto phrase_dictionary = get_locales().find(
      directions_options.language());

  // If language tag is not found then throw error
  if (phrase_dictionary == get_locales().end()) {
    throw std::runtime_error("Invalid language tag.");
  }

  // if a NarrativeBuilder is derived with specific code for a particular
  // language then add logic here and return derived NarrativeBuilder
  if (directions_options.language() == "cs-CZ") {
    return midgard::make_unique<NarrativeBuilder_csCZ>(
        directions_options, trip_path, phrase_dictionary->second);
  }

  // otherwise just return pointer to NarrativeBuilder
  return midgard::make_unique<NarrativeBuilder>(directions_options, trip_path,
                                                phrase_dictionary->second);
}

}
}
