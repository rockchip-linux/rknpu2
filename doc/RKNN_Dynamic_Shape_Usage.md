# RKNN动态形状输入使用说明
动态形状输入是指模型输入数据的形状在运行时可以改变。它可以帮助处理输入数据大小不固定的情况，增加模型的灵活性。动态形状输入在图像处理和序列模型推理中具有重要的作用。以下是动态形状输入的使用说明：

## RKNN SDK版本要求和限制
* RKNN-Toolkit2版本>=1.5.0
* RKNPU Runtime库(librknnrt.so)版本>=1.5.0
* RK3566/RK3568/RK3588/RK3588S/RK3562平台的NPU支持该功能

## 使用步骤：
## 1. 确认模型支持动态形状输入
首先，您需要确认模型支持动态形状输入。不是所有的模型都支持动态形状输，例如，常量的形状无法改变，RKNN-Toolkit2工具在转换过程会报错，因此您需要查看您的模型文件或者使用工具来确认模型是否支持动态形状输入。如果您的模型不支持动态形状输入，您需要重新训练模型，以支持动态形状输入。

## 2. 创建动态形状输入RKNN模型
在使用RKNN C API进行推理之前，需要先将模型转换成RKNN格式。您可以使用RKNN-Toolkit2工具来完成这个过程。如果您希望使用动态形状输入，可以设置转换出的RKNN模型可供使用的多个形状列表。对于多输入的模型，每个输入的形状个数要保持一致。例如，在使用RKNN-Toolkit2转换Caffe模型时，您可以使用以下代码：
```
    dynamic_input = [
        [[1,3,224,224]],    # set the first shape for all inputs
        [[1,3,192,192]],    # set the second shape for all inputs
        [[1,3,160,160]],    # set the third shape for all inputs
    ]

    # Pre-process config
    rknn.config(mean_values=[103.94, 116.78, 123.68], std_values=[58.82, 58.82, 58.82], quant_img_RGB2BGR=True, dynamic_input=dynamic_shapes)
```
这将告诉RKNN-Toolkit2生成支持3个形状的动态形状输入的RKNN模型。

完整的创建动态形状输入RKNN示例，请参考**https://github.com/rockchip-linux/rknn-toolkit2/tree/master/examples/functions/dynamic_input**

## 3. 查询RKNN模型支持的输入形状组合
得到动态形状输入RKNN模型后，接下来开始使用RKNPU C API进行部署，通过rknn_query可以查询到RKNN模型支持的输入形状列表,每个输入支持的形状列表信息以rknn_input_range结构体形式返回，它包含了每个输入的名称，layout信息，形状个数以及具体形状。例如，您可以使用以下代码：
```
    // 查询模型支持的输入形状
    rknn_input_range dyn_range[io_num.n_input];
    memset(dyn_range, 0, io_num.n_input * sizeof(rknn_input_range));
    for (uint32_t i = 0; i < io_num.n_input; i++)
    {
        dyn_range[i].index = i;
        ret = rknn_query(ctx, RKNN_QUERY_INPUT_DYNAMIC_RANGE, &dyn_range[i], sizeof(rknn_input_range));
        if (ret != RKNN_SUCC)
        {
            fprintf(stderr, "rknn_query error! ret=%d\n", ret);
            return -1;
        }
        dump_input_dynamic_range(&dyn_range[i]);
    }
```
注意：对于多输入的模型，所有输入的形状按顺序一一对应，例如，假设有两个输入、多种形状，第一个输入的第一个形状与第二个输入的第一个形状组合有效，不存在交叉的形状组合。


## 4.设置输入形状
加载动态形状输入RKNN模型后，您可以在运行时动态修改输入的形状。通过调用rknn_set_input_shapes接口，传入所有输入的rknn_tensor_attr数组,可以设置当前次推理的形状。例如，使用rknn_query获取的输入形状设置输入时，您可以使用以下代码：

```
    for (int s = 0; s < shape_num; ++s)
    {
        for (int i = 0; i < io_num.n_input; i++)
        {
            for (int j = 0; j < input_attrs[i].n_dims; ++j)
            {
                input_attrs[i].dims[j] = shape_range[i].dyn_range[s][j];
            }
        }
        ret = rknn_set_input_shapes(ctx, io_num.n_input, input_attrs);
        if (ret < 0)
        {
            fprintf(stderr, "rknn_set_input_shapes error! ret=%d\n", ret);
            return -1;
        }
    }
```
其中，shape_num是支持的形状个数，shape_range[i]是第i个输入的rknn_input_range结构体，io_num.n_input是输入数量,input_attrs是模型所有输入的rknn_tensor_attr结构体数组。

在设置输入形状后，可以再次调用rknn_query查询当前次推理成功设置后的输入和输出形状，例如，您可以使用以下代码：
```
        // 获取当前次推理的输入和输出形状
        rknn_tensor_attr cur_input_attrs[io_num.n_input];
        memset(cur_input_attrs, 0, io_num.n_input * sizeof(rknn_tensor_attr));
        for (uint32_t i = 0; i < io_num.n_input; i++)
        {
            cur_input_attrs[i].index = i;
            ret = rknn_query(ctx, RKNN_QUERY_CURRENT_INPUT_ATTR, &(cur_input_attrs[i]), sizeof(rknn_tensor_attr));
            if (ret < 0)
            {
                printf("rknn_init error! ret=%d\n", ret);
                return -1;
            }
            dump_tensor_attr(&cur_input_attrs[i]);
        }

        rknn_tensor_attr cur_output_attrs[io_num.n_output];
        memset(cur_output_attrs, 0, io_num.n_output * sizeof(rknn_tensor_attr));
        for (uint32_t i = 0; i < io_num.n_output; i++)
        {
            cur_output_attrs[i].index = i;
            ret = rknn_query(ctx, RKNN_QUERY_CURRENT_OUTPUT_ATTR, &(cur_output_attrs[i]), sizeof(rknn_tensor_attr));
            if (ret != RKNN_SUCC)
            {
                printf("rknn_query fail! ret=%d\n", ret);
                return -1;
            }
            dump_tensor_attr(&cur_output_attrs[i]);
        }
```

**注意**
对于动态形状输入RKNN模型,rknn_query接口暂不支持所有带NATIVE的输入输出属性查询命令。

## 进行推理
在设置输入形状之后，可以使用普通API或者零拷贝API进行推理。每次切换输入形状时，需要再设置一次新的形状，并设置对应形状的输入数据。以普通API为例，您可以使用以下代码进行推理：

```
    // 设置输入信息
    rknn_input inputs[io_num.n_input];
    memset(inputs, 0, io_num.n_input * sizeof(rknn_input));
    for (int i = 0; i < io_num.n_input; i++)
    {
        int height = cur_input_attrs[i].fmt == RKNN_TENSOR_NHWC ? cur_input_attrs[i].dims[1] : cur_input_attrs[i].dims[2];
        int width = cur_input_attrs[i].fmt == RKNN_TENSOR_NHWC ? cur_input_attrs[i].dims[2] : cur_input_attrs[i].dims[3];
        cv::resize(imgs[i], imgs[i], cv::Size(width, height));
        inputs[i].index = i;
        inputs[i].pass_through = 0;
        inputs[i].type = RKNN_TENSOR_UINT8;
        inputs[i].fmt = RKNN_TENSOR_NHWC;
        inputs[i].buf = imgs[i].data;
        inputs[i].size = imgs[i].total() * imgs[i].channels();
    }

    // 将输入数据转换成正确的格式后，放到输入缓冲区
    ret = rknn_inputs_set(ctx, io_num.n_input, inputs);
    if (ret < 0)
    {
        printf("rknn_input_set fail! ret=%d\n", ret);
        return -1;
    }

    // 进行推理
    printf("Begin perf ...\n");
    double total_time = 0;
    for (int i = 0; i < loop_count; ++i)
    {
        int64_t start_us = getCurrentTimeUs();
        ret = rknn_run(ctx, NULL);
        int64_t elapse_us = getCurrentTimeUs() - start_us;
        if (ret < 0)
        {
            printf("rknn run error %d\n", ret);
            return -1;
        }
        total_time += elapse_us / 1000.f;
        printf("%4d: Elapse Time = %.2fms, FPS = %.2f\n", i, elapse_us / 1000.f, 1000.f * 1000.f / elapse_us);
    }
    printf("Avg FPS = %.3f\n", loop_count * 1000.f / total_time);

    // 获取输出结果
    rknn_output outputs[io_num.n_output];
    memset(outputs, 0, io_num.n_output * sizeof(rknn_output));
    for (uint32_t i = 0; i < io_num.n_output; ++i)
    {
        outputs[i].want_float = 1;
        outputs[i].index = i;
        outputs[i].is_prealloc = 0;
    }

    ret = rknn_outputs_get(ctx, io_num.n_output, outputs, NULL);
    if (ret < 0)
    {
        printf("rknn_outputs_get fail! ret=%d\n", ret);
        return ret;
    }
```

总之，RKNN动态形状输入可以帮助您处理可变大小的输入数据，提高模型的灵活性和效率。通过以上步骤，您可以使用RKNN C API进行动态形状输入的推理。完整的动态形状输入C API Demo请参考**https://github.com/rockchip-linux/rknpu2/tree/master/examples/rknn_dynamic_shape_input_demo**