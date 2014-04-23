
int last_main();
S32 track( std::vector<VectorFeaturePoint> &Modellist,///已知条件
                itr_math::RectangleS rect_source,///已知条件
                Matrix &mat1,///测试匹配用（debug）
                Matrix &mat2,///待匹配图片
                Matrix &reMat,///测试匹配用（debug）
                itr_math::RectangleS &rect_result);
S32 center_cal(F32 *data,S32 length);
