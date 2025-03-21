/* SPDX-FileCopyrightText: 2013 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

/** \file
 * \ingroup depsgraph
 */

#include "intern/debug/deg_debug.h"

#include "BLI_console.h"
#include "BLI_hash.h"
#include "BLI_string.h"
#include "BLI_time.h"

#include "BKE_global.hh"

namespace blender::deg {

DepsgraphDebug::DepsgraphDebug() : flags(G.debug), graph_evaluation_start_time_(0) {}

bool DepsgraphDebug::do_time_debug() const
{
  return ((G.debug & G_DEBUG_DEPSGRAPH_TIME) != 0);
}

void DepsgraphDebug::begin_graph_evaluation()
{
  if (!do_time_debug()) {
    return;
  }

  const double current_time = BLI_time_now_seconds();

  graph_evaluation_start_time_ = current_time;
}

void DepsgraphDebug::end_graph_evaluation()
{
  if (!do_time_debug()) {
    return;
  }

  const double graph_eval_end_time = BLI_time_now_seconds();
  const double graph_eval_time = graph_eval_end_time - graph_evaluation_start_time_;

  if (name.empty()) {
    printf("Depsgraph updated in %f seconds.\n", graph_eval_time);
  }
  else {
    printf("Depsgraph [%s] updated in %f seconds.\n", name.c_str(), graph_eval_time);
  }
}

bool terminal_do_color()
{
  return (G.debug & G_DEBUG_DEPSGRAPH_PRETTY) != 0;
}

std::string color_for_pointer(const void *pointer)
{
  if (!terminal_do_color()) {
    return "";
  }
  int r, g, b;
  BLI_hash_pointer_to_color(pointer, &r, &g, &b);
  char buffer[64];
  SNPRINTF(buffer, TRUECOLOR_ANSI_COLOR_FORMAT, r, g, b);
  return std::string(buffer);
}

std::string color_end()
{
  if (!terminal_do_color()) {
    return "";
  }
  return std::string(TRUECOLOR_ANSI_COLOR_FINISH);
}

}  // namespace blender::deg
