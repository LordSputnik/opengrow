#include "og_pch.hpp"
#include "Desc.hpp"

namespace og
{
  void BranchDesc::SetFromNode(const pugi::xml_node & level)
  {
    pugi::xml_node attrib_set = level.child("mid");
    for( pugi::xml_node node = attrib_set.first_child(); node; node = node.next_sibling() )
    {
      if( strcmp( node.name(), "geotropism" ) == 0)
      {
        mid.light_attraction = StrToStd<float>( node.child_value() );
      }
      else if( strcmp( node.name(), "first_split" ) == 0)
      {
        mid.first_split_distance = StrToStd<float>( node.child_value() );
      }
      else if( strcmp( node.name(), "segment_length" ) == 0)
      {
        mid.segment_length = StrToStd<float>( node.child_value() );
      }
      else if( strcmp( node.name(), "split_frequency" ) == 0)
      {
        mid.split_frequency = StrToStd<float>( node.child_value() );
      }
      else if( strcmp( node.name(), "length" ) == 0)
      {
        mid.total_length = StrToStd<float>( node.child_value() );
      }
      else if( strcmp( node.name(), "split_angle" ) == 0)
      {
        mid.split_angle = StrToStd<float>( node.child_value() );
      }
      else if( strcmp( node.name(), "bud_growth_samples" ) == 0)
      {
        mid.bud_growth_samples = StrToStd<uint16_t>( node.child_value() );
      }
    }

    attrib_set = level.child("var");
    for( pugi::xml_node node = attrib_set.first_child(); node; node = node.next_sibling() )
    {
      if( strcmp( node.name(), "geotropism" ) == 0)
      {
        var.light_attraction = StrToStd<float>( node.child_value() );
      }
      else if( strcmp( node.name(), "first_split" ) == 0)
      {
        var.first_split_distance = StrToStd<float>( node.child_value() );
      }
      else if( strcmp( node.name(), "segment_length" ) == 0)
      {
        var.segment_length = StrToStd<float>( node.child_value() );
      }
      else if( strcmp( node.name(), "split_frequency" ) == 0)
      {
        var.split_frequency = StrToStd<float>( node.child_value() );
      }
      else if( strcmp( node.name(), "length" ) == 0)
      {
        var.total_length = StrToStd<float>( node.child_value() );
      }
      else if( strcmp( node.name(), "split_angle" ) == 0)
      {
        var.split_angle = StrToStd<float>( node.child_value() );
      }
      else if( strcmp( node.name(), "bud_growth_samples" ) == 0)
      {
        var.bud_growth_samples = StrToStd<uint16_t>( node.child_value() );
      }
    }
  }

  BranchDesc::BranchDesc(const pugi::xml_node & node)
  {
    SetFromNode(node);
  }

  BranchDesc::BranchDesc(const pugi::xml_node & node, const BranchDesc & defaults) :
    mid(defaults.mid),
    var(defaults.var)
  {
    SetFromNode(node);
  }

  TreeDesc::TreeDesc(const pugi::xml_node & tree_root)
  {
    species = tree_root.child_value("species");

    max_levels = StrToStd<uint8_t>(tree_root.child_value("max_levels"));

    // Allocate memory to hold the midpoint and variation for the values in each branch level.
    levels.resize(max_levels);

    const pugi::xml_node data_node = tree_root.child ( "data" );

    pugi::xml_node node = data_node.child ( "base_splits" );
    base_splits[MID] = StrToStd<float> ( node.child_value ( "mid" ) );
    base_splits[VAR] = StrToStd<float> ( node.child_value ( "var" ) );

    node = data_node.child ( "defaults" );

    defaults.reset(new BranchDesc(node));

    node = data_node.child ( "levels" );

    for( pugi::xml_node level = node.first_child(); level; level = level.next_sibling() )
    {
      size_t index = level.attribute ( "id" ).as_uint();

      if ( index >= max_levels )
        continue;

      /* Set the level to the default values */
      levels[index].reset(new BranchDesc(level,(*defaults)));
    }
  }
}
