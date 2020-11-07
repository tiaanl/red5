#pragma once

#include "engine/renderer/types.h"
#include "engine/renderer/vertex.h"

namespace engine {

class Renderer;

class MeshBuilder {
public:
  class VertexBuilder {
  public:
    VertexBuilder& position(F32 x, F32 y, F32 z = 0.0f) {
      m_vertex.position.x = x;
      m_vertex.position.y = y;
      m_vertex.position.z = z;

      return *this;
    }

    VertexBuilder& texCoord(F32 u, F32 v) {
      m_vertex.texCoord.x = u;
      m_vertex.texCoord.y = v;

      return *this;
    }

    VertexBuilder& color(F32 red, F32 green, F32 blue, F32 alpha) {
      m_vertex.color.x = red;
      m_vertex.color.y = green;
      m_vertex.color.z = blue;
      m_vertex.color.w = alpha;

      return *this;
    }

    VertexBuilder& color(const ColorF& color) {
      m_vertex.color.x = color.red;
      m_vertex.color.y = color.green;
      m_vertex.color.z = color.blue;
      m_vertex.color.w = color.alpha;

      return *this;
    }

    MeshBuilder& build() {
      m_meshBuilder.m_vertices.push_back(m_vertex);
      return m_meshBuilder;
    }

  private:
    friend class MeshBuilder;

    explicit VertexBuilder(MeshBuilder& meshBuilder)
      : m_meshBuilder{meshBuilder}, m_vertex{{}, {}, {}} {}

    MeshBuilder& m_meshBuilder;
    Vertex m_vertex;
  };

  static MeshBuilder create(RenderMode renderMode) {
    return MeshBuilder{renderMode};
  }

  VertexBuilder vertex() {
    return VertexBuilder(*this);
  }

  void submit(Renderer& renderer);

private:
  explicit MeshBuilder(RenderMode renderMode);

  RenderMode m_renderMode;
  std::vector<Vertex> m_vertices;
};

void renderRectangle(Renderer& renderer, const RectF& rect, const ColorF& color);
void renderRectangleOutline(Renderer& renderer, const RectF& rect, const ColorF& color);

}  // namespace engine
