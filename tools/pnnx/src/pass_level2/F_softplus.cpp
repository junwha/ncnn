// Tencent is pleased to support the open source community by making ncnn available.
//
// Copyright (C) 2021 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "pass_level2.h"

namespace pnnx {

class F_softplus : public GraphRewriterPass
{
public:
    const char* match_pattern_graph() const
    {
        return R"PNNXIR(7767517
5 4
pnnx.Input              input_0     0 1 input
pnnx.Input              input_1     0 1 beta
pnnx.Input              input_2     0 1 threshold
aten::softplus          op_0        3 1 input beta threshold out
pnnx.Output             output      1 0 out
)PNNXIR";
    }

    const char* type_str() const
    {
        return "F.softplus";
    }
};

REGISTER_GLOBAL_PNNX_GRAPH_REWRITER_PASS(F_softplus, 100)

class F_softplus_onnx : public GraphRewriterPass
{
public:
    const char* match_pattern_graph() const
    {
        return R"PNNXIR(7767517
3 2
pnnx.Input              input_0     0 1 input
Softplus                op_0        1 1 input out
pnnx.Output             output      1 0 out
)PNNXIR";
    }

    const char* type_str() const
    {
        return "F.softplus";
    }

    void write(Operator* op, const std::map<std::string, Parameter>& /*captured_params*/) const
    {
        op->params["beta"] = 1.f;
        op->params["threshold"] = 20.f;
    }
};

REGISTER_GLOBAL_PNNX_GRAPH_REWRITER_PASS(F_softplus_onnx, 101)

class F_softplus_onnx_1 : public GraphRewriterPass
{
public:
    const char* match_pattern_graph() const
    {
        return R"PNNXIR(7767517
7 6
pnnx.Input              input_0     0 1 input
prim::Constant          op_0        0 1 beta value=%beta
aten::mul               op_1        2 1 input beta a
Softplus                op_2        1 1 a b
prim::Constant          op_3        0 1 beta2 value=%beta
aten::div               op_4        2 1 b beta2 out
pnnx.Output             output      1 0 out
)PNNXIR";
    }

    const char* type_str() const
    {
        return "F.softplus";
    }

    void write(Operator* op, const std::map<std::string, Parameter>& captured_params) const
    {
        op->params["beta"] = captured_params.at("beta");
        op->params["threshold"] = 20.f;
    }
};

REGISTER_GLOBAL_PNNX_GRAPH_REWRITER_PASS(F_softplus_onnx_1, 100)

} // namespace pnnx
